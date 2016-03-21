README
============

**DEPRECATED PROJECT**

This project uses the old project ARP (ArchC Referece Plataform) to boot the Linux image 
using the SPARC processor. 

**You must use the ArchC 2.2.0.**

The ARP project was replaced by the [MPSoCBench project](https://github.com/ArchC/MPSoCBench). 

How to start this project:
---------------------------

* Download and install the ArchC 2.2.0:

```bash
git clone --branch v2.2.0 git@github.com:ArchC/ArchC.git 
cd ArchC
./boot
./configure --prefix=$ARCHC_INSTALL_DIR --with-systemc=$SYSTEMC_INSTALL_DIR --with-tlm=$SYSTEMC_INSTALL_DIR 
make
make install
```

* Compile the arp-leon

```bash
cd arp-leon
# Edit the Makefile.arp fixing the SYSTEMC and ARCHC path
make
```

* Execute the arp-leon

```bash
make run
```

* When the ``waiting for connection on port...`` appears, open a new terminal and type:

```bash
nc localhost 6000
```

* In the terminal you ran the platform, when the ``program load > `` appears, type:

```bash
../../sw/linux_images/linux-2.0.x.bin
```

Now, the platform will load a linux image. Other images can be loaded as well. See the contents of **sw/linux_images**. 

* After completing the boot, go to the terminal that you previously typed ``nc localhost 6000`` and enjoy
your guest linux system.









