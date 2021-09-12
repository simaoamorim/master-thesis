# Slave device source code

## Dependencies - Raspberry Pi OS Lite

- libgpiod2
- libgpiod-dev
- libi2c-dev
- netHAT 52-RTE driver (download DVD content from https://www.netiot.com/interface/nethat/)
  - V1.0.0.1/Driver/nxhat-drv-1.1.0.deb (main CIFX Driver)
  - V1.0.0.1/Firmware/nxhat-ecs-4.5.0.0.deb (for using the EtherCAT protocol)

Bash script to automatically install these dependencies (included at [/src/install-dependencies.sh](/src/install-dependencies.sh)):

```bash
sudo apt-get install libgpiod2 libgpiod-dev libi2c-dev
wget https://www.netiot.com/fileadmin/big_data/en-US/Resources/zip/netHAT_DVD_2016-08-1_V1_0_0_1.zip
unzip netHAT_DVD_2016-08-1_V1_0_0_1.zip
sudo dpkg -i V1.0.0.1/Driver/nxhat-drv-1.1.0.deb
sudo dpkg -i V1.0.0.1/Firmware/nxhat-ecs-4.5.0.0.deb
```

#### Useful tools (mostly for debbugging)
 - i2c-tools
