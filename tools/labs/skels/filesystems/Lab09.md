# Laborator 9

## myfs

### Exercitiul 1
Am identificat versiunea de kernel de pe masina pentru a gasi versiunea corecta a `ramfs` pe care am folosit-o ca model.

```
root@qemux86:~# cat /proc/version
Linux version 5.10.14+
```

Am completat structura `struct inode_operations` cu operatii specifice inode-ului de tip director: `mknod`, `create` si `mkdir`; aceste functii au fost apoi implementate dupa modelul din `ramfs`. Pentru celelalte functii am folosit varianta generica din VFS.

## Exercitiul 2
Completand structura struct `file_operations` am adaugat operatiile specifice lucrului cu structura `file` asociata inode-ului folosind tot implementarile generice din VFS. Am adaugat in structura `inode_operations` operatiile pentru manipulare inode-ului si structura `address_space_operations`. Dupa initializare am asignat structurile completate anterior campurilor potrivite din structura de inode.

Am testat corectitudinea implementarii astfel:
```
root@qemux86:~/skels/filesystems/myfs# insmod myfs.ko
myfs: loading out-of-tree module taints kernel.
root@qemux86:~/skels/filesystems/myfs# mkdir -p /mnt/myfs
root@qemux86:~/skels/filesystems/myfs# mount -t myfs none /mnt/myfs
root inode has 2 link(s)
root@qemux86:~/skels/filesystems/myfs# touch /mnt/myfs/peanuts.txt
mknod begin
mknod end
root@qemux86:~/skels/filesystems/myfs# mkdir -p /mnt/myfs/mountain/forest
mknod begin
mknod end
mknod begin
mknod end
root@qemux86:~/skels/filesystems/myfs# touch /mnt/myfs/mountain/forest/tree.txt
mknod begin
mknod end
root@qemux86:~/skels/filesystems/myfs# rm /mnt/myfs/mountain/forest/tree.txt
root@qemux86:~/skels/filesystems/myfs# rmdir /mnt/myfs/mountain/forest
root@qemux86:~/skels/filesystems/myfs# echo "chocolate" > /mnt/myfs/peanuts.txt
root@qemux86:~/skels/filesystems/myfs# cat /mnt/myfs/peanuts.txt
chocolate
root@qemux86:~/skels/filesystems/myfs# cat /mnt/myfs/peanuts.txt
```

## minfs
