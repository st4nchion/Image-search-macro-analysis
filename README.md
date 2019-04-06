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
  
