{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 25.0, 69.0, 1093.0, 675.0 ],
		"bglocked" : 0,
		"defrect" : [ 25.0, 69.0, 1093.0, 675.0 ],
		"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
		"openinpresentation" : 0,
		"default_fontsize" : 12.0,
		"default_fontface" : 0,
		"default_fontname" : "Arial",
		"gridonopen" : 0,
		"gridsize" : [ 15.0, 15.0 ],
		"gridsnaponopen" : 0,
		"toolbarvisible" : 1,
		"boxanimatetime" : 200,
		"imprint" : 0,
		"enablehscroll" : 1,
		"enablevscroll" : 1,
		"devicewidth" : 0.0,
		"boxes" : [ 			{
				"box" : 				{
					"maxclass" : "toggle",
					"outlettype" : [ "int" ],
					"numinlets" : 1,
					"patching_rect" : [ 619.0, 138.0, 20.0, 20.0 ],
					"id" : "obj-26",
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "unique $1",
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"numinlets" : 2,
					"patching_rect" : [ 619.353394, 166.0, 54.0, 16.0 ],
					"id" : "obj-24",
					"numoutlets" : 1,
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "getndevices",
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"numinlets" : 2,
					"patching_rect" : [ 681.353394, 166.0, 64.0, 16.0 ],
					"id" : "obj-18",
					"numoutlets" : 1,
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"numinlets" : 2,
					"patching_rect" : [ 449.0, 267.0, 50.0, 19.0 ],
					"id" : "obj-8",
					"numoutlets" : 1,
					"fontsize" : 13.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "You can specify the type for the depth matrix (long, float32 or float64)",
					"linecount" : 2,
					"fontname" : "Arial",
					"numinlets" : 1,
					"patching_rect" : [ 515.469849, 195.0, 174.0, 29.0 ],
					"id" : "obj-1",
					"numoutlets" : 0,
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"varname" : "copyright-comment[1]",
					"text" : "convert to char",
					"fontname" : "Arial",
					"numinlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 0.901961 ],
					"patching_rect" : [ 182.0, 264.0, 111.0, 18.0 ],
					"frgb" : [ 0.0, 0.0, 0.0, 0.901961 ],
					"id" : "obj-23",
					"numoutlets" : 0,
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"outlettype" : [ "", "" ],
					"numinlets" : 1,
					"patching_rect" : [ 178.0, 330.0, 80.0, 60.0 ],
					"id" : "obj-21",
					"numoutlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "jit.matrix 4 char 640 480",
					"outlettype" : [ "jit_matrix", "" ],
					"fontname" : "Arial",
					"numinlets" : 1,
					"patching_rect" : [ 178.0, 304.0, 139.0, 20.0 ],
					"id" : "obj-14",
					"numoutlets" : 2,
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.fpsgui",
					"outlettype" : [ "", "" ],
					"fontname" : "Arial",
					"numinlets" : 1,
					"patching_rect" : [ 681.0, 582.0, 80.0, 36.0 ],
					"id" : "obj-13",
					"numoutlets" : 2,
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"outlettype" : [ "", "" ],
					"numinlets" : 1,
					"patching_rect" : [ 681.0, 328.0, 320.0, 240.0 ],
					"id" : "obj-11",
					"numoutlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"rounded" : 15,
					"numinlets" : 1,
					"bgcolor" : [ 0.0, 0.0, 0.0, 0.101961 ],
					"patching_rect" : [ 673.0, 320.0, 336.0, 256.0 ],
					"id" : "obj-12",
					"numoutlets" : 0,
					"border" : 1,
					"bordercolor" : [ 0.501961, 0.501961, 0.501961, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"varname" : "output-comment[1]",
					"text" : "Output 2",
					"fontname" : "Arial",
					"numinlets" : 1,
					"patching_rect" : [ 780.5, 257.0, 52.0, 19.0 ],
					"id" : "obj-3",
					"numoutlets" : 0,
					"fontsize" : 11.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"varname" : "output-description[1]",
					"text" : "4-plane char (rgb)",
					"fontname" : "Arial",
					"numinlets" : 1,
					"patching_rect" : [ 838.0, 257.0, 175.0, 19.0 ],
					"id" : "obj-4",
					"numoutlets" : 0,
					"fontsize" : 11.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"varname" : "output-panel[1]",
					"rounded" : 0,
					"numinlets" : 1,
					"bgcolor" : [ 0.615686, 0.607843, 0.607843, 0.2 ],
					"patching_rect" : [ 779.0, 252.0, 240.0, 29.0 ],
					"id" : "obj-7",
					"numoutlets" : 0,
					"border" : 1,
					"bordercolor" : [ 1.0, 1.0, 1.0, 0.647059 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"varname" : "output-comment",
					"text" : "Output 1",
					"fontname" : "Arial",
					"numinlets" : 1,
					"patching_rect" : [ 779.5, 226.0, 52.0, 19.0 ],
					"id" : "obj-31",
					"numoutlets" : 0,
					"fontsize" : 11.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"varname" : "input-comment",
					"text" : "Input",
					"fontname" : "Arial",
					"numinlets" : 1,
					"patching_rect" : [ 788.0, 191.0, 35.0, 19.0 ],
					"id" : "obj-34",
					"numoutlets" : 0,
					"fontsize" : 11.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"varname" : "input-description",
					"text" : "bang",
					"fontname" : "Arial",
					"numinlets" : 1,
					"patching_rect" : [ 838.0, 191.0, 175.0, 19.0 ],
					"id" : "obj-37",
					"numoutlets" : 0,
					"fontsize" : 11.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"varname" : "output-description",
					"text" : "1-plane long, Float32 or Float64 (depth)",
					"linecount" : 2,
					"fontname" : "Arial",
					"numinlets" : 1,
					"patching_rect" : [ 838.0, 219.0, 175.0, 32.0 ],
					"id" : "obj-38",
					"numoutlets" : 0,
					"fontsize" : 11.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"varname" : "io-cat-panel",
					"rounded" : 0,
					"numinlets" : 1,
					"bgcolor" : [ 0.556863, 0.556863, 0.556863, 0.2 ],
					"patching_rect" : [ 779.0, 186.0, 56.0, 95.0 ],
					"id" : "obj-39",
					"numoutlets" : 0,
					"border" : 1,
					"bordercolor" : [ 1.0, 1.0, 1.0, 0.647059 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"varname" : "output-panel",
					"rounded" : 0,
					"numinlets" : 1,
					"bgcolor" : [ 0.615686, 0.607843, 0.607843, 0.2 ],
					"patching_rect" : [ 779.0, 215.0, 240.0, 37.0 ],
					"id" : "obj-42",
					"numoutlets" : 0,
					"border" : 1,
					"bordercolor" : [ 1.0, 1.0, 1.0, 0.647059 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"varname" : "input-panel",
					"rounded" : 0,
					"numinlets" : 1,
					"bgcolor" : [ 0.615686, 0.607843, 0.607843, 0.2 ],
					"patching_rect" : [ 778.0, 186.0, 240.0, 29.0 ],
					"id" : "obj-44",
					"numoutlets" : 0,
					"border" : 1,
					"bordercolor" : [ 1.0, 1.0, 1.0, 0.647059 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"varname" : "io-panel",
					"rounded" : 0,
					"numinlets" : 1,
					"bgcolor" : [ 1.0, 1.0, 1.0, 0.403922 ],
					"patching_rect" : [ 773.0, 181.0, 251.0, 111.0 ],
					"id" : "obj-5",
					"numoutlets" : 0,
					"border" : 1,
					"bordercolor" : [ 0.0, 0.0, 0.0, 0.517647 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"outlettype" : [ "int", "bang" ],
					"minimum" : 0,
					"maximum" : 3,
					"triscale" : 0.9,
					"fontname" : "Arial",
					"numinlets" : 1,
					"patching_rect" : [ 446.353394, 142.0, 48.0, 18.0 ],
					"id" : "obj-53",
					"numoutlets" : 2,
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "mode $1",
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"numinlets" : 2,
					"patching_rect" : [ 446.353394, 166.0, 49.0, 16.0 ],
					"id" : "obj-57",
					"numoutlets" : 1,
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "0: raw data (0 - 2047)\r1:  depth (b-w);\r2: depth (w-b);\n3: distance (in cm)\r",
					"linecount" : 4,
					"fontname" : "Arial",
					"numinlets" : 1,
					"patching_rect" : [ 501.469849, 131.0, 110.0, 52.0 ],
					"id" : "obj-99",
					"numoutlets" : 0,
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "cvjit_random_bg_colour",
					"outlettype" : [ "int", "int", "int" ],
					"fontname" : "Arial",
					"hidden" : 1,
					"numinlets" : 0,
					"patching_rect" : [ 60.0, 434.0, 117.0, 18.0 ],
					"id" : "obj-6",
					"numoutlets" : 3,
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "bgcolor",
					"fontname" : "Arial",
					"hidden" : 1,
					"numinlets" : 4,
					"patching_rect" : [ 60.0, 458.546204, 165.5, 18.0 ],
					"id" : "obj-10",
					"numoutlets" : 0,
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"varname" : "copyright-comment",
					"text" : "Copyright ©2010",
					"fontname" : "Arial",
					"numinlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 0.901961 ],
					"patching_rect" : [ 52.0, 595.0, 111.0, 18.0 ],
					"frgb" : [ 0.0, 0.0, 0.0, 0.901961 ],
					"id" : "obj-17",
					"numoutlets" : 0,
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"varname" : "email-comment",
					"text" : "jmp@jmpelletier.com\n",
					"fontname" : "Arial",
					"numinlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 0.901961 ],
					"patching_rect" : [ 186.0, 530.0, 105.0, 18.0 ],
					"frgb" : [ 0.0, 0.0, 0.0, 0.901961 ],
					"id" : "obj-19",
					"numoutlets" : 0,
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"varname" : "me-comment",
					"text" : "By Jean-Marc Pelletier,\nNenad Popov \nand Andrew Roth\n",
					"linecount" : 3,
					"fontname" : "Arial",
					"numinlets" : 1,
					"patching_rect" : [ 51.0, 530.0, 139.0, 48.0 ],
					"id" : "obj-22",
					"numoutlets" : 0,
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"varname" : "copyright-panel",
					"numinlets" : 1,
					"bgcolor" : [ 1.0, 1.0, 1.0, 0.403922 ],
					"patching_rect" : [ 46.0, 525.0, 250.0, 95.0 ],
					"id" : "obj-49",
					"numoutlets" : 0,
					"border" : 1,
					"bordercolor" : [ 0.0, 0.0, 0.0, 0.647059 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"varname" : "comments-panel",
					"rounded" : 0,
					"numinlets" : 1,
					"bgcolor" : [ 1.0, 1.0, 1.0, 0.647059 ],
					"patching_rect" : [ 60.0, 105.0, 250.0, 80.0 ],
					"id" : "obj-65",
					"numoutlets" : 0,
					"border" : 1,
					"bordercolor" : [ 0.0, 0.0, 0.0, 0.517647 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.fpsgui",
					"outlettype" : [ "", "" ],
					"fontname" : "Arial",
					"numinlets" : 1,
					"patching_rect" : [ 340.0, 581.0, 80.0, 36.0 ],
					"id" : "obj-70",
					"numoutlets" : 2,
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"outlettype" : [ "", "" ],
					"numinlets" : 1,
					"patching_rect" : [ 340.0, 328.0, 320.0, 240.0 ],
					"id" : "obj-67",
					"numoutlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Use live camera input",
					"fontname" : "Arial",
					"numinlets" : 1,
					"textcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"patching_rect" : [ 365.0, 117.0, 116.0, 19.0 ],
					"frgb" : [ 1.0, 1.0, 1.0, 1.0 ],
					"id" : "obj-59",
					"numoutlets" : 0,
					"fontsize" : 11.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "toggle",
					"outlettype" : [ "int" ],
					"numinlets" : 1,
					"patching_rect" : [ 340.0, 116.0, 20.0, 20.0 ],
					"id" : "obj-50",
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "qmetro 2",
					"outlettype" : [ "bang" ],
					"fontname" : "Arial",
					"numinlets" : 2,
					"patching_rect" : [ 340.0, 143.0, 50.0, 18.0 ],
					"id" : "obj-48",
					"numoutlets" : 1,
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "close",
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"numinlets" : 2,
					"patching_rect" : [ 406.0, 166.0, 34.0, 16.0 ],
					"id" : "obj-46",
					"numoutlets" : 1,
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "open",
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"numinlets" : 2,
					"patching_rect" : [ 367.0, 166.0, 33.0, 16.0 ],
					"id" : "obj-45",
					"numoutlets" : 1,
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "jit.freenect.grab float32 @mode 1",
					"outlettype" : [ "", "", "" ],
					"fontname" : "Arial",
					"numinlets" : 1,
					"patching_rect" : [ 340.0, 199.0, 159.0, 18.0 ],
					"id" : "obj-43",
					"numoutlets" : 3,
					"fontsize" : 10.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"rounded" : 0,
					"numinlets" : 1,
					"bgcolor" : [ 0.0, 0.0, 0.0, 0.0 ],
					"patching_rect" : [ 30.0, 60.0, 778.0, 4.0 ],
					"id" : "obj-20",
					"numoutlets" : 0,
					"border" : 1,
					"bordercolor" : [ 0.0, 0.0, 0.0, 0.207843 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Jitter external using the libfreenect library to grab from a Kinect device",
					"fontname" : "Arial",
					"numinlets" : 1,
					"textcolor" : [ 0.258824, 0.258824, 0.258824, 1.0 ],
					"patching_rect" : [ 60.0, 60.0, 732.0, 29.0 ],
					"frgb" : [ 0.258824, 0.258824, 0.258824, 1.0 ],
					"id" : "obj-16",
					"numoutlets" : 0,
					"fontsize" : 20.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "jit.freenect.grab",
					"fontname" : "Arial",
					"numinlets" : 1,
					"textcolor" : [ 0.258824, 0.258824, 0.258824, 1.0 ],
					"patching_rect" : [ 60.0, 30.0, 565.0, 41.0 ],
					"frgb" : [ 0.258824, 0.258824, 0.258824, 1.0 ],
					"id" : "obj-15",
					"fontface" : 1,
					"numoutlets" : 0,
					"fontsize" : 30.028469
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"rounded" : 15,
					"numinlets" : 1,
					"bgcolor" : [ 0.0, 0.0, 0.0, 0.101961 ],
					"patching_rect" : [ 332.0, 320.0, 336.0, 256.0 ],
					"id" : "obj-147",
					"numoutlets" : 0,
					"border" : 1,
					"bordercolor" : [ 0.501961, 0.501961, 0.501961, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"rounded" : 30,
					"numinlets" : 1,
					"bgcolor" : [ 0.0, 0.0, 1.0, 0.05098 ],
					"patching_rect" : [ 335.0, 195.0, 179.0, 29.0 ],
					"id" : "obj-9",
					"numoutlets" : 0,
					"border" : 1,
					"bordercolor" : [ 0.0, 0.0, 1.0, 0.733333 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"rounded" : 0,
					"numinlets" : 1,
					"bgcolor" : [ 0.0, 0.0, 0.0, 0.2 ],
					"patching_rect" : [ 330.0, 105.0, 428.0, 130.0 ],
					"id" : "obj-58",
					"numoutlets" : 0,
					"border" : 1,
					"bordercolor" : [ 0.501961, 0.501961, 0.501961, 1.0 ]
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-67", 0 ],
					"destination" : [ "obj-70", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-50", 0 ],
					"destination" : [ "obj-48", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-10", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 1 ],
					"destination" : [ "obj-10", 1 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 2 ],
					"destination" : [ "obj-10", 2 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-53", 0 ],
					"destination" : [ "obj-57", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-11", 0 ],
					"destination" : [ "obj-13", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-14", 0 ],
					"destination" : [ "obj-21", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-43", 0 ],
					"destination" : [ "obj-14", 0 ],
					"hidden" : 0,
					"midpoints" : [ 349.5, 294.0, 187.5, 294.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-43", 1 ],
					"destination" : [ "obj-11", 0 ],
					"hidden" : 0,
					"midpoints" : [ 419.5, 307.0, 690.5, 307.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-57", 0 ],
					"destination" : [ "obj-43", 0 ],
					"hidden" : 0,
					"midpoints" : [ 455.853394, 188.5, 349.5, 188.5 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-43", 0 ],
					"destination" : [ "obj-67", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-46", 0 ],
					"destination" : [ "obj-43", 0 ],
					"hidden" : 0,
					"midpoints" : [ 415.5, 188.0, 349.5, 188.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-45", 0 ],
					"destination" : [ "obj-43", 0 ],
					"hidden" : 0,
					"midpoints" : [ 376.5, 188.0, 349.5, 188.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-48", 0 ],
					"destination" : [ "obj-43", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-43", 2 ],
					"destination" : [ "obj-8", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 0 ],
					"destination" : [ "obj-43", 0 ],
					"hidden" : 0,
					"midpoints" : [ 690.853394, 189.0, 349.5, 189.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-26", 0 ],
					"destination" : [ "obj-24", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-24", 0 ],
					"destination" : [ "obj-43", 0 ],
					"hidden" : 0,
					"midpoints" : [ 628.853394, 190.0, 349.5, 190.0 ]
				}

			}
 ]
	}

}
