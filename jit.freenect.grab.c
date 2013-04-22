/*
 Copyright 2010, Jean-Marc Pelletier, Nenad Popov and Andrew Roth 
 jmp@jmpelletier.com
 
 This file is part of jit.freenect.grab.
  
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
 http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 
 */

#include "jit.common.h"
#include "ext_systhread.h"
#include "ext_obex.h"
#include <libusb.h>
#include "libfreenect.h"
#include "freenect_internal.h"
#include <time.h>

#define IR_WIDTH 1280
#define IR_HEIGHT 1024
#define MAX_DEVICES 8

// TODO: always check log level before release:
#define JIT_FREENECT_LOG_LEVEL	FREENECT_LOG_DEBUG
//FREENECT_LOG_FLOOD
#define NESADEBUG

#ifdef NESADEBUG
#   define postNesa(fmt, ...) cpost((fmt), ##__VA_ARGS__);
#warning Nesa's debug log enabled
#else
#   define postNesa(...)
#endif

#ifdef NESADEBUGFLOOD
#   define postNesaFlood(fmt, ...) cpost((fmt), ##__VA_ARGS__);
#warning Nesa's debug flood log enabled
#else
#   define postNesaFlood(...)
#endif

#define DEBUG_TIMESTAMP __DATE__" "__TIME__"\x0"

typedef float float4[4];
#define mult_scalar_float4(a,b,c) {int vector_iterator; for(vector_iterator=0;vector_iterator<4;vector_iterator++)c[vector_iterator] = a[vector_iterator] * b;}

typedef union _lookup_data{
	long *l_ptr;
	float *f_ptr;
	double *d_ptr;
}t_lookup;

enum thread_mess_type{
	NONE,
	OPEN,
	CLOSE,
	TERMINATE
};


typedef struct _jit_freenect_grab
{
	t_object         ob;
	char             unique;
	float            threshold;
	char             has_frames;
	long             index;
	long             ndevices;
	freenect_device  *device;
	uint32_t         timestamp;
	t_lookup         lut;
	t_symbol         *lut_type;
	long             tilt;
	long             accelcount;
	double           mks_accel[3];
	//uint8_t          *ir_data;
	//uint16_t         *depth_data;
	// back: owned by libfreenect (implicit for depth)
	// mid: owned by callbacks, "latest frame ready"
	// front: owned by GL, "currently being drawn"
	uint8_t *ir_back, *ir_mid, *ir_front;
	uint32_t         ir_timestamp;
	int got_ir;
	boolean_t			 is_open;
	char             have_ir_frames;
	float            *ir;
	freenect_raw_tilt_state *state;
	
					// thread reference
	t_systhread_mutex backbuffer_mutex;

	int				x_sleeptime;	
	int				id;

    //
    //pthread_mutex_t  cb_mutex;
} t_jit_freenect_grab;

/*typedef struct _obj_list
{
	t_jit_freenect_grab **objects;
	uint32_t count;
} t_obj_list;
*/

void *_jit_freenect_grab_class;

#pragma mark - Globals 

//int object_count = 0;


int global_id;

freenect_context *f_ctx;
t_systhread		x_systhread;
boolean_t		x_systhread_cancel;				// thread cancel flag
boolean_t		freenect_active;
int open_device_count;

#pragma mark - Forward Defintions

t_jit_err               jit_freenect_grab_init(void);
t_jit_freenect_grab     *jit_freenect_grab_new(void);
void                    jit_freenect_grab_free(t_jit_freenect_grab *x);

void                    jit_freenect_grab_open(t_jit_freenect_grab *x, t_symbol *s, long argc, t_atom *argv);
void                    jit_freenect_grab_close(t_jit_freenect_grab *x, t_symbol *s, long argc, t_atom *argv);

t_jit_err               jit_freenect_grab_get_ndevices(t_jit_freenect_grab *x, void *attr, long *ac, t_atom **av);
t_jit_err               jit_freenect_grab_get_accel(t_jit_freenect_grab *x, void *attr, long *ac, t_atom **av);
t_jit_err               jit_freenect_grab_get_tilt(t_jit_freenect_grab *x, void *attr, long *ac, t_atom **av);
void					jit_freenect_grab_set_tilt(t_jit_freenect_grab *x,  void *attr, long argc, t_atom *argv);

t_jit_err               jit_freenect_grab_matrix_calc(t_jit_freenect_grab *x, void *inputs, void *outputs);
//void                    build_geometry(t_jit_freenect_grab *x, void *matrix, char *out_bp, t_jit_matrix_info *dest_info);
void                    copy_ir_data(uint8_t *source, char *out_bp, t_jit_matrix_info *dest_info);

void                    ir_callback(freenect_device *dev, void *pixels, uint32_t timestamp);

void *jit_freenect_capture_threadproc();//t_jit_freenect_grab *x);
long jit_freenect_restart_thread(t_jit_freenect_grab *x);
void jit_freenect_thread_sleeptime(t_jit_freenect_grab *x, long sleeptime);
void jit_freenect_thread_stop(t_jit_freenect_grab *x);
void jit_freenect_thread_cancel(t_jit_freenect_grab *x);

void calculate_lut(t_lookup *lut, t_symbol *type, int mode){
	long i;
	
	if(type == _jit_sym_float32){
		float *f_lut;
		f_lut = (float *)realloc(lut->f_ptr, sizeof(float) * 0x800);
		if(!f_lut){
			error("Out of memory!");
			return;
		}
		lut->f_ptr = f_lut;
		
		switch(mode){
			case 0:
				for(i=0;i<0x800;i++){
					lut->f_ptr[i] = (float)i;
				}
				break;
			case 1:
				for(i=0;i<0x800;i++){
					lut->f_ptr[i] = (float)i * (1.f / (float)0x7FF);
				}
				break;
			case 2:
				for(i=0;i<0x800;i++){
					lut->f_ptr[i] = 1.f - ((float)i * (1.f / (float)0x7FF));
				}
				break;
			case 3:
				postNesa("making mode 3 lut\n"); //TODO: remove
				for(i=0;i<0x7FF;i++){
					lut->f_ptr[i] =  (float)i*0.01f;//(float)(-10.f / (3.33f + (float)i * -0.00307f));
				}
				lut->f_ptr[0x800]=-1.f;
				break;
			case 4:
				postNesa("making mode 4 lut\n"); //TODO: remove
				for(i=0;i<0x800;i++){
					lut->f_ptr[i] = 10.f / (3.33f + (float)i * -0.00307f);
				} 
				break;
		}
	}
	else if(type == _jit_sym_long){
		long *l_lut;
		l_lut = (long *)realloc(lut->l_ptr, sizeof(long) * 0x800);
		if(!l_lut){
			error("Out of memory!");
			return;
		}
		lut->l_ptr = l_lut;
		
		switch(mode){
			case 0:
			case 1:
				for(i=0;i<0x800;i++){
					lut->l_ptr[i] = i;
				}
				break;
			case 2:
				for(i=0;i<0x800;i++){
					lut->l_ptr[i] = 0x7FF - i;
				}
				break;
			case 3:
				for(i=0;i<0x800;i++){
					lut->l_ptr[i] = (long)(-10.f / (3.33f + (float)i * -0.00307f));
				} 
				break;
			case 4:
				for(i=0;i<0x800;i++){
					lut->l_ptr[i] = (long)(-10.f / (3.33f + (float)i * -0.00307f));
				} 
				break;
		}
	}
	else if(type == _jit_sym_float64){
		double *d_lut;
		d_lut = (double *)realloc(lut->d_ptr, sizeof(double) * 0x800);
		if(!d_lut){
			error("Out of memory!");
			return;
		}
		lut->d_ptr = d_lut;
		
		switch(mode){
			case 0:
				for(i=0;i<0x800;i++){
					lut->d_ptr[i] = (double)i;
				}
				break;
			case 1:
				for(i=0;i<0x800;i++){
					lut->d_ptr[i] = (double)i * (1.0 / (double)0x7FF);
				}
				break;
			case 2:
				for(i=0;i<0x800;i++){
					lut->d_ptr[i] = 1.0 - ((double)i * (1.0 / (double)0x7FF));
				}
				break;
			case 3:
				for(i=0;i<0x800;i++){
					lut->d_ptr[i] = -10.0 / (3.33 + (double)i * -0.00307);
				} 
				break;
			case 4:
				for(i=0;i<0x800;i++){
					lut->d_ptr[i] = -10.0 / (3.33 + (double)i * -0.00307);
				} 
				break;
		}
	}
	else if(type == NULL){
		if(lut->f_ptr){
			free(lut->f_ptr);
			lut->f_ptr = NULL;
		}
	}
	else{
		error("Invalid type for lookup table calculation. char not supported.");
		return;
	}
}



t_jit_err jit_freenect_grab_init(void)
{
	long attrflags=0;
	t_jit_object *attr;
	t_jit_object *mop,*output;
	t_atom a[4];
	int i;
	
	global_id=0;
	x_systhread = NULL;
	x_systhread_cancel=FALSE;
	freenect_active=FALSE;
	open_device_count=0;
		
	_jit_freenect_grab_class = jit_class_new("jit_freenect_grab",(method)jit_freenect_grab_new,
											 (method)jit_freenect_grab_free, sizeof(t_jit_freenect_grab),0L);
  	
	//add mop
	mop = (t_jit_object *)jit_object_new(_jit_sym_jit_mop,0,1); //0 inputs, 1 outputs
		
	//Prepare IR image
	output = jit_object_method(mop,_jit_sym_getoutput,1);
	
	jit_atom_setsym(a,_jit_sym_char); //default
	jit_object_method(output,_jit_sym_types,1,a);
	
	jit_attr_setlong(output,_jit_sym_minplanecount,1);
	jit_attr_setlong(output,_jit_sym_maxplanecount,1);
	
	jit_atom_setlong(&a[0], IR_WIDTH);
	jit_atom_setlong(&a[1], IR_HEIGHT);
	
	jit_object_method(output, _jit_sym_mindim, 2, a);
	jit_object_method(output, _jit_sym_maxdim, 2, a);
	
	jit_class_addadornment(_jit_freenect_grab_class,mop);
	
	//add methods
	jit_class_addmethod(_jit_freenect_grab_class, (method)jit_freenect_grab_open, "open", A_GIMME, 0L);
	jit_class_addmethod(_jit_freenect_grab_class, (method)jit_freenect_grab_close, "close", A_GIMME, 0L);
	
	jit_class_addmethod(_jit_freenect_grab_class, (method)jit_freenect_grab_matrix_calc, "matrix_calc", A_CANT, 0L);
	
	//add attributes	
	attrflags = JIT_ATTR_GET_DEFER_LOW | JIT_ATTR_SET_USURP_LOW;
	attr = (t_jit_object *)jit_object_new(_jit_sym_jit_attr_offset,"unique",_jit_sym_char,
										  attrflags,(method)NULL,(method)NULL,calcoffset(t_jit_freenect_grab,unique));
	jit_attr_addfilterset_clip(attr,0,1,TRUE,TRUE);
	jit_class_addattr(_jit_freenect_grab_class,attr);
		
	attr = (t_jit_object *)jit_object_new(_jit_sym_jit_attr_offset,"tilt",_jit_sym_long,
										  attrflags,(method)jit_freenect_grab_get_tilt,(method)jit_freenect_grab_set_tilt,
										  calcoffset(t_jit_freenect_grab,tilt));
	jit_class_addattr(_jit_freenect_grab_class,attr);
	
	
	attrflags = JIT_ATTR_GET_DEFER_LOW | JIT_ATTR_SET_OPAQUE;
	attr = (t_jit_object *)jit_object_new(_jit_sym_jit_attr_offset,"index",_jit_sym_long,
										  attrflags,(method)NULL,(method)NULL,calcoffset(t_jit_freenect_grab,index));
	jit_class_addattr(_jit_freenect_grab_class,attr);
	
	attr = (t_jit_object *)jit_object_new(_jit_sym_jit_attr_offset,"ndevices",_jit_sym_long,
										  attrflags,(method)jit_freenect_grab_get_ndevices,(method)NULL,calcoffset(t_jit_freenect_grab,ndevices));
	jit_class_addattr(_jit_freenect_grab_class,attr);
	
	attr = (t_jit_object *)jit_object_new(_jit_sym_jit_attr_offset_array, "accel", _jit_sym_float64, 3, 
										  attrflags, (method)jit_freenect_grab_get_accel,(method)NULL, 
										  calcoffset(t_jit_freenect_grab, accelcount),calcoffset(t_jit_freenect_grab,mks_accel));
	jit_class_addattr(_jit_freenect_grab_class,attr);

	
	attrflags = JIT_ATTR_GET_OPAQUE_USER | JIT_ATTR_SET_OPAQUE_USER;
	attr = (t_jit_object *)jit_object_new(_jit_sym_jit_attr_offset,"has_frames",_jit_sym_char,
										  attrflags,(method)NULL,(method)NULL,calcoffset(t_jit_freenect_grab,has_frames));
	jit_class_addattr(_jit_freenect_grab_class,attr);
	
	jit_class_register(_jit_freenect_grab_class);
	


	post("jit.freenect.grab: Copyright 2010, Jean-Marc Pelletier, Nenad Popov and Andrew Roth. Built on %s",DEBUG_TIMESTAMP);
	return JIT_ERR_NONE;
}

t_jit_freenect_grab *jit_freenect_grab_new(void)
{
	t_jit_freenect_grab *x;
	
	if ((x=(t_jit_freenect_grab *)jit_object_alloc(_jit_freenect_grab_class)))
	{
		x->device = NULL;
		x->timestamp = 0;
		x->unique = 0;
		x->has_frames = 0;
		x->ndevices = 0;
		x->lut.f_ptr = NULL;
		x->lut_type = NULL;
		x->tilt = 0;
		x->state = NULL;
		x->threshold = 2.f;
		x->ir = NULL;
        
		//x->x_systhread = NULL;
		x->backbuffer_mutex = NULL;
		x->x_sleeptime = 10;
		systhread_mutex_new(&x->backbuffer_mutex, 0);
		x->got_ir=0;

		x->ir_front=NULL;
		x->ir_back=NULL;
		x->ir_mid=NULL;

		
		x->ir_back = (uint8_t*)malloc(IR_WIDTH*IR_HEIGHT);
		x->ir_mid = (uint8_t*)malloc(IR_WIDTH*IR_HEIGHT);
		x->ir_front = (uint8_t*)malloc(IR_WIDTH*IR_HEIGHT);
		
		x->is_open=FALSE;
		x->id=++global_id;
		
		//jit_fnect_restart_thread(x);
        //pthread_mutex_init(&x->cb_mutex, NULL);

		postNesa("new freenect instance added.");//TODO: remove	
	} else {
		x = NULL;
	}
	return x;
}

void jit_freenect_grab_free(t_jit_freenect_grab *x)
{
	postNesa("grab_free called");
	// this call stops the thread if all devices are closed.
	postNesa("grab_free:calling grab_close");
	jit_freenect_grab_close(x, NULL, 0, NULL);
	
	jit_freenect_thread_stop(x);

	// free out mutex
	if (x->backbuffer_mutex)
		systhread_mutex_free(x->backbuffer_mutex);

	if (x->ir_back) free(x->ir_back);
	if (x->ir_mid) free(x->ir_mid);
	if (x->ir_front) free(x->ir_front);
			
	if(x->lut.f_ptr){
		free(x->lut.f_ptr);
	}
	
	//release_cloud(&x->cloud);
}

t_jit_err jit_freenect_grab_get_ndevices(t_jit_freenect_grab *x, void *attr, long *ac, t_atom **av){
	
	if ((*ac)&&(*av)) {
		
	} else {
		*ac = 1;
		if (!(*av = jit_getbytes(sizeof(t_atom)*(*ac)))) {
			*ac = 0;
			return JIT_ERR_OUT_OF_MEM;
		}
	}
	
	if(f_ctx){
		x->ndevices = freenect_num_devices(f_ctx);
	}
	else{
		x->ndevices = 0;
	}
	
	
	jit_atom_setlong(*av,x->ndevices);
	
	return JIT_ERR_NONE;
}


t_jit_err jit_freenect_grab_get_accel(t_jit_freenect_grab *x, void *attr, long *ac, t_atom **av){
	double ax=0,ay=0,az=0;
	
	if ((*ac)&&(*av)) {
		
	} else {
		*ac = 3;
		if (!(*av = jit_getbytes(sizeof(t_atom)*(*ac)))) {
			*ac = 0;
			return JIT_ERR_OUT_OF_MEM;
		}
	}

	if(x->device){
		freenect_update_tilt_state(x->device);
		x->state = freenect_get_tilt_state(x->device);
		if (x->state)
			freenect_get_mks_accel(x->state, &ax, &ay, &az);
	}
	
	jit_atom_setfloat(*av, ax);
	jit_atom_setfloat(*av +1, ay);
	jit_atom_setfloat(*av +2, az);
		
	return JIT_ERR_NONE;
}

t_jit_err jit_freenect_grab_get_tilt(t_jit_freenect_grab *x, void *attr, long *ac, t_atom **av){
	double tilt=0;
	
	
	if ((*ac)&&(*av)) {
		
	} else {
		*ac = 1;
		if (!(*av = jit_getbytes(sizeof(t_atom)*(*ac)))) {
			*ac = 0;
			return JIT_ERR_OUT_OF_MEM;
		}
	}
	
	if(x->device){
		freenect_update_tilt_state(x->device);
		x->state = freenect_get_tilt_state(x->device);
		if (x->state)
			tilt = freenect_get_tilt_degs(x->state);
		
	}
	
	jit_atom_setfloat(*av, tilt);
	
	
	return JIT_ERR_NONE;
}

void jit_freenect_grab_set_tilt(t_jit_freenect_grab *x,  void *attr, long argc, t_atom *argv)
{
	if(argv){
		x->tilt = jit_atom_getlong(argv);
		
		CLIP(x->tilt, -30, 30);
		
		if(x->device){
			freenect_set_tilt_degs(x->device,x->tilt);
		}
	}
}

void jit_freenect_grab_open(t_jit_freenect_grab *x,  t_symbol *s, long argc, t_atom *argv)
{
	int ndevices, devices_left, dev_ndx;
	t_jit_freenect_grab *y;
	freenect_device *dev;
	
	postNesa("opening device...\n");//TODO: remove
	
	if(x->device){
		error("A device is already open.");
		return;
	}
	x->is_open = FALSE;
	if(!f_ctx){
		
		postNesa("!f_ctx is null, opening a new device\n");//TODO: remove
		
		if (jit_freenect_restart_thread(x)!=MAX_ERR_NONE) {
			
		//if (pthread_create(&capture_thread, NULL, capture_threadfunc, NULL)) {
			error("Failed to create capture thread.");
			return;
		}
		int bailout=0;
		while((!f_ctx)&&(++bailout<1000)){
			//systhread_sleep(1);
			sleep(0);
			//post("deadlocking in the sun %i",bailout);//TODO: remove
		}
		if (!f_ctx)
		{
			// TODO: replace with conditionall
			error("Failed to init freenect after %i retries.\n",bailout);
			return;
		}
	}
	
	ndevices = freenect_num_devices(f_ctx);
	
	if(!ndevices){
		error("Could not find any connected Kinect device.");
		return;
	}
	
	devices_left = ndevices;
	dev = f_ctx->first;
	while(dev){
		dev = dev->next;
		devices_left--;
	}
	
	if(!devices_left){
		error("All Kinect devices are currently in use.");
		return;
	}
	
	if(!argc){
		x->index = 0;	
	}
	else{
		//Is the device already in use?
		x->index = jit_atom_getlong(argv);
		
		dev = f_ctx->first;
		while(dev){
			y = freenect_get_user(dev);
			if(y->index == x->index){
				error("Kinect device %d is already in use.", x->index);
				x->index = 0;
				return;
			}
			dev = dev->next;
		}
	}
	
	if(x->index > ndevices){
		error("Cannot open Kinect device %d, only %d are connected.", x->index, ndevices);
		x->index = 0;
		return;
	}
	
	//Find out which device to open
	dev_ndx = x->index;
	if(!dev_ndx){
		int found = 0;
		while(!found){
			found = 1;
			dev = f_ctx->first;
			while(dev){
				y = freenect_get_user(dev);
				if(y->index-1 == dev_ndx){
					found = 0;
					break;
				}
				dev = dev->next;
			}
			dev_ndx++;
		}
		x->index = dev_ndx;
	}
		
	if (freenect_open_device(f_ctx, &(x->device), dev_ndx-1) < 0) {
		error("Could not open Kinect device %d", dev_ndx);
		x->index = 0;
		x->device = NULL;
	}
		else {
			postNesa("device open");//TODO: remove
		}

	//freenect_set_depth_buffer(x->device, x->depth_back);
	//freenect_set_video_buffer(x->device, x->ir_back);
	
	freenect_set_video_callback(x->device, ir_callback);
		freenect_set_video_mode(x->device, freenect_find_video_mode(FREENECT_RESOLUTION_HIGH, FREENECT_VIDEO_IR_8BIT));
	
	
	//Store a pointer to this object in the freenect device struct (for use in callbacks)
	freenect_set_user(x->device, x);  
	
	freenect_set_led(x->device,LED_RED);
	
	//freenect_set_tilt_degs(x->device,x->tilt);
	
	//freenect_start_depth(x->device);
	//freenect_stop_depth(x->device);
	freenect_start_video(x->device);
	
	x->is_open = TRUE;
	open_device_count++;
	freenect_active=TRUE;
}

void jit_freenect_grab_close(t_jit_freenect_grab *x,  t_symbol *s, long argc, t_atom *argv)
{
	
	postNesa("closing device: start...");//TODO:r
	
	if(f_ctx)
	{
		postNesa("closing device:f_ctx is valid.");//TODO:r
		if(!f_ctx->first){
		//if (x->x_systhread){
			postNesa("closing device:last device, stopping thread...");
			jit_freenect_thread_stop(x);
		}
		
		postNesa("closing device freenect side...");//TODO:r
		if(!x->device) 
		{
			postNesa("closing device:device not open");//TODO:r
			return;
		}
		else {
			//freenect_stop_depth(x->device);
			//freenect_stop_video(x->device);
			freenect_set_led(x->device,LED_BLINK_GREEN);
			freenect_close_device(x->device);
			x->device = NULL;
			open_device_count--;
		}


	}
	else {
		postNesa("closing device:f_ctx is null, nothing to close");//TODO:r
	}

	

postNesa("closing device:done\n");//TODO:r	
}

t_jit_err jit_freenect_grab_matrix_calc(t_jit_freenect_grab *x, void *inputs, void *outputs)
{
	t_jit_err err=JIT_ERR_NONE;
	long depth_savelock=0,ir_savelock=0;
	t_jit_matrix_info ir_minfo;
	void *ir_matrix;
	char *ir_bp;
	
	uint8_t *tmp8;
	
	int has_new_frame = 0;
	
	ir_matrix = jit_object_method(outputs,_jit_sym_getindex,0); 
	
	if (x && ir_matrix) {
		
		postNesaFlood("matrixcalc:isopem=%i",x->is_open);
		if (x->is_open)
		{
			systhread_mutex_lock(x->backbuffer_mutex);
			
			
			if (x->got_ir>0) {
				tmp8 = x->ir_front;
				x->ir_front = x->ir_mid;
				x->ir_mid = tmp8;
				x->got_ir = 0;
				has_new_frame=1;
			}
			systhread_mutex_unlock(x->backbuffer_mutex);
		}
		else {
			postNesaFlood("matrixcalc:device not open");
		}
		
		ir_savelock = (long) jit_object_method(ir_matrix,_jit_sym_lock,1);
		
		if(!x->device){
			goto out;
		}
		
		jit_object_method(ir_matrix,_jit_sym_getinfo,&ir_minfo);
		
		if (ir_minfo.type != _jit_sym_char)
		{
			err=JIT_ERR_MISMATCH_TYPE;
			goto out;
		}
		
			if(ir_minfo.planecount != 1){
				ir_minfo.planecount = 1;
				jit_object_method(ir_matrix, _jit_sym_setinfo, &ir_minfo);
				jit_object_method(ir_matrix, _jit_sym_getinfo, &ir_minfo);
			}
		

		jit_object_method(ir_matrix,_jit_sym_getdata,&ir_bp);
		if (!ir_bp) { err=JIT_ERR_INVALID_OUTPUT; goto out;}
		
		 
		//Grab and copy matrices
/*		x->has_frames = 0;  //Assume there are no new frames
		
		if(x->have_ir_frames || x->have_depth_frames){
			x->timestamp = MAX(x->ir_timestamp,x->depth_timestamp);
			
			if(x->have_ir_frames){
				copy_ir_data(x->ir_data, ir_bp, &ir_minfo);
				x->have_ir_frames = 0;
				//x->has_frames = 1;
			}
			
			if(x->have_depth_frames){
				//if(x->mode == 4){
				//	build_geometry(x, depth_matrix, depth_bp, &depth_minfo);
				//}
				//else{
					copy_depth_data(x->depth_data, depth_bp, &depth_minfo, &x->lut);
				//}
				x->have_depth_frames = 0;
				x->has_frames = 1;
			}
			else if((x->clear_depth)&&((x->ir_timestamp - x->depth_timestamp)>3000000)){
				jit_object_method(depth_matrix, _jit_sym_clear);
				x->has_frames = 1;
				post("Clearing depth...");//TODO:r
			}
		}
*/		
		
		if (x->is_open)
		{
			x->has_frames=has_new_frame;
			
			if(has_new_frame>0)
			{
				copy_ir_data(x->ir_front, ir_bp, &ir_minfo);
			}
			
		}
		else {
			postNesa("device not open");//TODO:r
		}

		
	} else {
		return JIT_ERR_INVALID_PTR;
	}
	
out:
	jit_object_method(ir_matrix,gensym("lock"),ir_savelock);
	//systhread_mutex_unlock(x->x_mutex);
	return err;
}


void copy_ir_data(uint8_t *source, char *out_bp, t_jit_matrix_info *dest_info)
{
	int i,j;
	
	char *out;
	uint8_t *in;
	
	if(!source){
		return;
	}
	
	if(!out_bp || !dest_info){
		error("Invalid pointer in copy_ir_data.");
		return;
	}
	
	in = source;
	
		for(i=0;i<IR_HEIGHT;i++){
			out = out_bp + dest_info->dimstride[1] * i;
			for(j=0;j<IR_WIDTH;j++){
				*out = *in;
				
				out ++;
				in ++;
			
		}
	}
}

void ir_callback(freenect_device *dev, void *pixels, uint32_t timestamp){
	t_jit_freenect_grab *x;
	
	x = freenect_get_user(dev);
	
	if(!x)	
	{
		error("Invalid max object supplied in ir_callback\n");// TODO:should print only in debug mode
		return;
    }
		
	

	if (x->is_open)
	{
	systhread_mutex_lock(x->backbuffer_mutex);
	//pthread_mutex_lock(&x->cb_mutex);
	
	/*
	x->ir_data = pixels;
	x->ir_timestamp = timestamp;
	x->have_ir_frames++;
	*/
	
	//assert(x->ir_back == pixels);
	x->ir_back = x->ir_mid;
	freenect_set_video_buffer(dev, x->ir_back);
	x->ir_mid = (uint8_t*)pixels;
	x->got_ir++;
	
    systhread_mutex_unlock(x->backbuffer_mutex);
	}
    //pthread_mutex_unlock(&x->cb_mutex);
	
}



#pragma mark - Threading Stuff

long jit_freenect_restart_thread(t_jit_freenect_grab *x)
{
	long rval = MAX_ERR_NONE;
	
	postNesa("restarting thread.\n");//TODO: remove
	
	jit_freenect_thread_stop(x);								// kill thread if, any

	// create new thread + begin execution
	if (x_systhread == NULL) {
		postNesa("starting a new thread");
		x_systhread_cancel = FALSE;
		rval = systhread_create((method) jit_freenect_capture_threadproc, NULL, 0, 0, 0, &x_systhread);
	}
	
	return rval;
}

/*
 void max2thread(t_jit_tis_grab *x, long foo) 
 {
 systhread_mutex_lock(x->x_mutex);
 x->x_foo = foo;																// override our current value
 systhread_mutex_unlock(x->x_mutex);	
 }
 */

void jit_freenect_thread_sleeptime(t_jit_freenect_grab *x, long sleeptime) 
{
	if (sleeptime<10)
		sleeptime = 10;
	x->x_sleeptime = sleeptime;														// no need to lock since we are readonly in worker thread
}


void jit_freenect_thread_stop(t_jit_freenect_grab *x) 
{
	unsigned int ret;
	
	if (x_systhread) {
		postNesa("jit_freenect_thread_stop:stopping our thread");
		x_systhread_cancel = TRUE;						// tell the thread to stop		
		postNesa("jit_freenect_thread_stop:wait for the thread to stop");
		systhread_join(x_systhread, &ret);					// wait for the thread to stop
		x_systhread = NULL;
	}
	postNesa("jit_freenect_thread_stop:done");
}


void jit_freenect_thread_cancel(t_jit_freenect_grab *x) 
{	
	jit_freenect_thread_stop(x);									// kill thread if, any
	//outlet_anything(x->x_outlet, gensym("cancelled"), 0, NULL);	
}

void *jit_freenect_capture_threadproc()//t_jit_freenect_grab *x) 
{
	freenect_context *context;
	
	postNesa("Threadproc called");//TODO:r
	
	if(!f_ctx){
		postNesa("f_ctx is null, calling init_Freenect");//TODO:r
		if (freenect_init(&context, NULL) < 0) {
			error("freenect_init() failed");
			goto out;
		}
		freenect_set_log_level(context, JIT_FREENECT_LOG_LEVEL);
		postNesa("freenect_init ok,id");//TODO: remove
	}
	
	f_ctx = context;
	
	struct timeval timeout;
	timeout.tv_sec = 60;
	timeout.tv_usec = 0;
	
	// loop until told to stop
	while (1) {
		
		//postNesaFlood("threadid=%i",x->id);
		// test if we're being asked to die, and if so return before we do the work
		if (x_systhread_cancel)
		{
			postNesa("stopping thread");
			break;
		}
		// this thread is used only to process freenect events
		// no need to lock the mutex
		//systhread_mutex_lock(x->x_mutex);	

		//if(f_ctx->first){
			//if(freenect_process_events(f_ctx) < 0){
		
		//if(freenect_active)
		//{
		if(f_ctx->first)
		if(freenect_process_events_timeout(f_ctx, &timeout)<0)
			{
				error("Could not process events.");
				//if (x->x_systhread_cancel) 
				break;
			}
		//}
		//else {
		//	postNesa("freenect_active=false\n");
		//}


		//}
		//else {
		//	post("not first");//TODO:remove
		//}
		//x->x_foo++;																// fiddle with shared data
		
		
		//systhread_mutex_unlock(x->x_mutex);
		
		//qelem_set(x->x_qelem);													// notify main thread using qelem mechanism
		
		//usleep(2500);
		//systhread_sleep(x->x_sleeptime);						// sleep a bit
	}

out:
	postNesa("Threadproc exits");//TODO:r
	
	
	//postNesa("Threadproc calls grab_close ,id=%i\n",x->id);//TODO:r
	// TODO: add notification through dupmoutlet
	//jit_freenect_grab_close(x, NULL, 0, NULL);
	
	freenect_shutdown(f_ctx);
	f_ctx = NULL;
	
	
	//x->x_systhread_cancel = false;							// reset cancel flag for next time, in case
	// the thread is created again
	
	systhread_exit(0);															// this can return a value to systhread_join();
	return NULL;
}

// TODO: delete
/*
void *capture_threadfuncOLD(void *arg)
{	
	freenect_context *context;
	
	if(!f_ctx){
		if (freenect_init(&context, NULL) < 0) {
			printf("freenect_init() failed\n");
			goto out;
		}
	}
	
	f_ctx = context;
	
	terminate_thread = 0;
	
	while(!terminate_thread){
		if(f_ctx->first){
			if(freenect_process_events(f_ctx) < 0){
				error("Could not process events.");
				break;
			}
		}
		
		sleep(0);
	}
	
out:
	freenect_shutdown(f_ctx);
	f_ctx = NULL;
	//pthread_exit(NULL);
	x->x_systhread_cancel = false;							// reset cancel flag for next time, in case
	// the thread is created again
	
	systhread_exit(0);		
	return NULL;
}
*/