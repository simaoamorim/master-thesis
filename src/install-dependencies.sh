apt-get install libgpiod2 libgpiod-dev libi2c-dev
wget https://www.netiot.com/fileadmin/big_data/en-US/Resources/zip/netHAT_DVD_2016-08-1_V1_0_0_1.zip
unzip netHAT_DVD_2016-08-1_V1_0_0_1.zip
dpkg -i V1.0.0.1/Driver/nxhat-drv-1.1.0.deb
dpkg -i V1.0.0.1/Firmware/nxhat-ecs-4.5.0.0.deb
rm -r V1.0.0.1
rm netHAT_DVD_2016-08-1_V1_0_0_1.zip
