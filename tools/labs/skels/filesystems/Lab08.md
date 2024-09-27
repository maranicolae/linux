# Laborator  8

## myfs
### Exercitiul 1
Structura `file_system_type` a fost initializata pentru sistemul de fisiere virtual `myfs`. In functia de mount specifica acestuia am folosit `mount_nodev` intrucat filesystem-ul este virtual si nu are nevoie de suport fizic. In functiile de init si exit ale modulului am inregistrat/eliminat sistemul de fisiere.

Corectitudinea implementarii am verificat-o prin urmatoarea serie de comenzi in masina virtuala:

```
root@qemux86:~/skels/filesystems/myfs# insmod myfs.ko
myfs: loading out-of-tree module taints kernel.
register_filesystem successful
root@qemux86:~/skels/filesystems/myfs# cat /proc/filesystems | grep myfs
nodev   myfs
root@qemux86:~/skels/filesystems/myfs# mkdir -p /mnt/myfs
root@qemux86:~/skels/filesystems/myfs# mount -t myfs none /mnt/myfs
root inode has 1 link(s)
mount: mounting none on /mnt/myfs failed: Not a directory

```

### Exercitiul 2
In structura de tipul `super_operations` am definit functiile superblock-ului utilizandu-le pe cele generice `generic_delete_inode()` si `simple_statfs()` pentru o functionalitate minimala. Dupa aceea am completat functia `fill_super` completand restul campurilor din superblock.

### Exercitiul 3
Completand functia `myfs_get_inode` am initializat inode-ul radacina. Prelunad informatiile din namespace-ul root am completat campurile `uid`, `gid` si `mode` ale inode-ului. Fiind un director, am adaugat operatiile specifice si am incrementat numarul de link-uri.

### Exercitiul 4
Corectitudinea rezolvarii se poate observa din outputul urmator:

```
root@qemux86:~/skels/filesystems/myfs# mkdir -p /mnt/myfs
root@qemux86:~/skels/filesystems/myfs# mount -t myfs none /mnt/myfs
root inode has 2 link(s)
root@qemux86:~/skels/filesystems/myfs# cat /proc/mounts | grep myfs
none /mnt/myfs myfs rw,relatime 0 0
root@qemux86:~/skels/filesystems/myfs# ls -di /mnt/myfs/
   4282 /mnt/myfs/
root@qemux86:~/skels/filesystems/myfs# stat -f /mnt/myfs
  File: "/mnt/myfs"
    ID: 0        Namelen: 255     Type: UNKNOWN
Block size: 4096
Blocks: Total: 0          Free: 0          Available: 0
Inodes: Total: 0          Free: 0
root@qemux86:~/skels/filesystems/myfs# ls -la /mnt/myfs
drwxr-xr-x    2 root     root             0 Apr 30 12:52 .
drwxr-xr-x    3 root     root          1024 Apr 28 10:36 ..
root@qemux86:~/skels/filesystems/myfs# touch /mnt/myfs/a.txt
touch: /mnt/myfs/a.txt: Permission denied
root@qemux86:~/skels/filesystems/myfs# umount /mnt/myfs
```

## minfs

### Exercitiul 1
In functia de mount `myfs_mount` am folosit `mount_bdev` intrucat filesystem-ul este nu mai este virtual ci are nevoie de suport fizic. Campul `fs_flags` al structurii de tipul `file_system_type` are valoarea `FS_REQUIRES_DEV` pentru a indica un filesystem ce utilizeaza un disk.

Verificam ca filesystem-ul a fost creat cu succes:
```
root@qemux86:~/skels/filesystems/minfs/kernel# cat /proc/filesystems | grep minfs
nodev   minfs
```
## Exercitiul 2
Cu ajutorul functiei `sb_bread` citim block-ul de la index 0, campului `b_data` i se face cast la structura de tip superblock `struct minfs_super_block` (structura custom) si se verifica valoarea magic number-ului. Se copiaza valorile obtinute in superblock-ul generic si in structura de tipul `struct minfs_sb_info`.

## Exercitiul 3
Am completat functiile `minfs_alloc_inode` populand campul `vfs_inode` al structurii alocate folosind functia `inode_init_once`. Conform functiei utilizate,initializare se face o singura data, campurile inode-ului fiind **idempotent** (nu se modifica in urma operatiilor). Apoi am completat functia de dezalocare asociata.

## Exercitiul 4
Cu ajutorul functiei `sb_bread` citim block-ul de la index 1, apoi extragem inode-ul de index-ul `ino`. Am completat campurile `uid`, `gid`, `mode` si `size` ale variabilei `inode` (VFS inode). In cazul in care inode-ul este director, atunci se completeaza si campurile `i_op` si `i_fop` si incrementam numarul de link-uri.

Acum inlocuim apelul functiei `myfs_get_inode` cu aceasta functie `minfs_iget`.

## Exercitiul 5
Testam corectitudinea implementarii:

Setup:
```
root@qemux86:~/skels/filesystems/minfs/user# ./mkfs.minfs /dev/vdd
root@qemux86:~/skels/filesystems/minfs/user# cd
root@qemux86:~# cd skels/filesystems/minfs/kernel/
root@qemux86:~/skels/filesystems/minfs/kernel# insmod minfs.ko
minfs: loading out-of-tree module taints kernel.
root@qemux86:~/skels/filesystems/minfs/kernel# mkdir -p /mnt/minfs/
```
```
root@qemux86:~/skels/filesystems/minfs/user# ./test-minfs.sh
+ insmod ../kernel/minfs.ko
+ mkdir -p /mnt/minfs
+ ./mkfs.minfs /dev/vdb
+ mount -t minfs /dev/vdb /mnt/minfs
released superblock resources
mount: mounting /dev/vdb on /mnt/minfs failed: Not a directory
root@qemux86:~/skels/filesystems/minfs/user#
```
