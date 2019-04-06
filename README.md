# Analysis-and-Summary

<pre><code>
Made by Lapid, Cat-Security

If you want to use somthing in this documents, please state the source.

thk
</code></pre>
* * *

Which macro?
--------------------
Many people play games and use macros through android emulators.
Especially, lots of macros are operating based on **image search** and these macros cause great financial damage to game makers and companies.
So I wanted to attach it to an Android emulator to create a module that could prevent these macros.


HOW??
-------------------
Most macros with image search function are usually operated using **AHK or opencv**.
So while searching for macro that contain this function, i found dll that user created their own for make easier to use in AHK.

<center><img src = "https://user-images.githubusercontent.com/41680753/55665052-85283980-5873-11e9-9f32-5cf163f7636d.png"></center>

Now let's use IDA PRO and x64 debugger to analyze that DLL.


ANALYSIS
--------------------

![imagesearch_param](https://user-images.githubusercontent.com/41680753/55665309-084b8e80-5878-11e9-83a5-cd6a5c32cde0.PNG)

The above picture shows the function of image search and information about the factors of the function.

* **Parameter Explain**
  - str loc : directory name where the image file to find is located
  - str image : image name that want to find (exclude Extensions)
  - int x : x-coordinate in the upper left hand corner of the image
  - int y : y-coordinate in the upper left hand corner of the image
  - uint _hWnd : the handle of the window in which the image is searched
  - double acc : accuracy of image searching
  - int click_flag : flag for click or not when image is detected (0 == not click / 1 == click) 
  --> __this parameter is not important__
  
**1. How does the function bring information about the image it wants to find?**
------------------------------------------------------------------------------
**All actions are operated within the above function**

  ![getcurrentdirectoryW](https://user-images.githubusercontent.com/41680753/55665601-a2620580-587d-11e9-9456-3bdc6e109101.PNG)
  
  Gets the absolute path of the macro is exist (current process) and stores it in BUFFER with a size of 100.
  
  [detail information about GetCurrentDirectoryW] : https://docs.microsoft.com/en-us/windows/desktop/api/winbase/nf-winbase-getcurrentdirectory
  
  ![source_image_append_bmp](https://user-images.githubusercontent.com/41680753/55667034-21613900-5892-11e9-94b0-d1a74af7c5f6.PNG)
  
  1. Save the directory name that contain image want to find in location_1
  2. Append the image name to the location_1 (directory name) and Save in location_append_image_name
  3. Append the extension "bmp" to the location_append_image_name and Save in v21
  4. Store information from v21 in v55
  
  ![abs_loction](https://user-images.githubusercontent.com/41680753/55665888-8ceeda80-5881-11e9-9ef7-8defd070b6f3.PNG)
  
  Save source image variable by combining V55 with the absolute path that was stored in the buffer.
  Then the absolute path for which we want to find images is completed.
  
  ![loadimagew](https://user-images.githubusercontent.com/41680753/55665969-eacff200-5882-11e9-8a6c-4d1e2d34758f.PNG)

  Loads the stand-alone image from the file specified by lpszName (image that we want to find) and Save in bitmap variable.
  
  [detaile information about LoadImageW] : https://docs.microsoft.com/en-us/windows/desktop/api/winuser/nf-winuser-loadimagew
  
  ![source_image_dc](https://user-images.githubusercontent.com/41680753/55666234-8b73e100-5886-11e9-9ff4-600ae603743a.PNG)
  
  Let's look inside the function sub_18060DED0.
  
  ![compatibledc](https://user-images.githubusercontent.com/41680753/55666309-07baf400-5888-11e9-9f37-abc290a5babf.PNG)
  
  1. In line 2, Creates a memory DC compatible with the application's current screen and Save the handle to a memory DC in v3.
  2. Then, After V3 and V2 are passed as arguments, return a handle to the object being replaced.
  
  [detaile information about createcompatibledc] : https://docs.microsoft.com/en-us/windows/desktop/api/wingdi/nf-wingdi-createcompatibledc
  
  [detaile information about selectobject] : https://docs.microsoft.com/en-us/windows/desktop/api/wingdi/nf-wingdi-selectobject

  ![getdibits](https://user-images.githubusercontent.com/41680753/55666467-4a7dcb80-588a-11e9-87b9-26d28a59d5e7.PNG)
  
  After get v3 and v2 as arguments (hdc and hbitmap) and Operate, Save in a pointer to a bmi structure that specifies the desired format for the DIB data.
  
  [detaile information about GetDIBits] : https://docs.microsoft.com/en-us/windows/desktop/api/wingdi/nf-wingdi-getdibits
  
  ![create_opencv_class](https://user-images.githubusercontent.com/41680753/55666556-af85f100-588b-11e9-9778-5894436bf889.png)
  
  Put v5 and BMI.BMIHEADER.BIWIDTH into the arguments of the function.
  And in Concurrency::details::SchedulerBase::Create function 
  
  ![create_opencv_class_2](https://user-images.githubusercontent.com/41680753/55666574-f83daa00-588b-11e9-940e-b0d6c3d58a78.png)
  
  Using the v5 and bmi.bmiHeader.biwidth received by arguments, calculate the operation in the cvCreateData function and return the value to v26.
  
  **So far, we've analyzed the parts that bring information about the images we want to find.**
  
**2. How does the function get information about the window to which the image is to be found?**
------------------------------------------------------------------------------
**All actions are operated within the above function**

![memu_1](https://user-images.githubusercontent.com/41680753/55667537-70aa6800-5898-11e9-92f6-c45834877fbc.PNG)

Using GetWindowDc function and argument hWnda(handle of android emulator), retrieve the android emulator's device context and store a handle to a device context for the specialized window in the HDC variable.

![memu_2](https://user-images.githubusercontent.com/41680753/55667538-71db9500-5898-11e9-8192-d180ba002f2f.PNG)

And then hdc, hWnda and variables needed for operation are passed on to __printwindow function. 

![memu_3](https://user-images.githubusercontent.com/41680753/55667541-73a55880-5898-11e9-9b64-ad89908e9a83.PNG)

In __printwindow function, we can see a configuration similar to when we bring information about the image we want to find.
Among them, we can see in the second row that we haven't seen before.
This function creates a bitmap compatible with the device that is associated with the specified device context.
* **Arguments Explain**
  - v9 : DC of andriod emulator
  - v6 : bitmap width, in pixels
  - v8 : bitmap height, in pixels
After opration, this function return a handle to the compatible bitmap

Other functions operate the same as before.

![memu_4](https://user-images.githubusercontent.com/41680753/55667809-3a6ee780-589c-11e9-99c6-4a7a6f5fbfc9.PNG)

