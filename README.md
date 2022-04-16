# LWM-Trainer
A lightweight memory trainer preset

## Note:  
If you wan't to attach your program to a x86 process you have to compile in x86.  
If you wan't to attach your program to a x64 process you have to compile in x64.  

If you are using Visual Studio you have to put the following _CRT_SECURE_NO_WARNINGS under C/C++ -> Preprocessor -> Preprocessor definitions  
The program must have administrative rights  
### Important
#### Don't do this mistake!
<pre> object.Write("Name",12) </pre>  
#### Write this instead:  
<pre> object.Write("Name",int(12)) </pre>
#### This notation makes sure that you don't use a wrong type, which will result in a hard to debug undefined behaivior!
