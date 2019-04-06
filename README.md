# Analysis-and-Summary

<pre><code>
Made by Lapid, Cat-Security

If you want to use the source code, please state the source.
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
  ![getcurrentdirectoryW](https://user-images.githubusercontent.com/41680753/55665601-a2620580-587d-11e9-9456-3bdc6e109101.PNG)
  
  Gets the absolute path of the macro is exist (current process) and stores it in BUFFER with a size of 100.
  
  [detail information about GetCurrentDirectoryW] : https://docs.microsoft.com/en-us/windows/desktop/api/winbase/nf-winbase-getcurrentdirectory
  
  ![source_image_append_bmp](https://user-images.githubusercontent.com/41680753/55665804-595f8080-5880-11e9-9b95-788419e59251.PNG)
  
  1. Save the directory name that contain image want to find in location_1
  2. Append the image name to the location_1 (directory name) and Save in location_append_image_name
  3. Append the extension "bmp" to the v20 and Save in v21
  4. Store information from v21 in v55 through sprintf
  
  ![abs_loction](https://user-images.githubusercontent.com/41680753/55665888-8ceeda80-5881-11e9-9ef7-8defd070b6f3.PNG)
  
  Save source image variable by combining V55 with the absolute path that was stored in the buffer.
  Then the absolute path for which we want to find images is completed.
  
  ![loadimagew](https://user-images.githubusercontent.com/41680753/55665969-eacff200-5882-11e9-8a6c-4d1e2d34758f.PNG)

  Loads the stand-alone image from the file specified by lpszName (image that we want to find) and Save in bitmap variable.
  
  [detaile information about LoadImageW] : https://docs.microsoft.com/en-us/windows/desktop/api/winuser/nf-winuser-loadimagew
  
  ![source_image_dc](https://user-images.githubusercontent.com/41680753/55666234-8b73e100-5886-11e9-9ff4-600ae603743a.PNG)
