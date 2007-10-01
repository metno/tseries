#!/bin/sh

export LD_LIBRARY_PATH=/metno/local/lib/mesa:$LD_LIBRARY_PATH
  
SUBNET=`/sbin/ifconfig eth0 | grep "inet addr" | cut -d. -f3`

case $SUBNET in
    20)                       region="FOU" ;;
    36|40)                    region="VV"  ;;
    48)                       region="VNN" ;;
### 50)                       region="MA"  ;;
    16|18|24|26|56|90|104|50) region="VA"  ;;
    *)                        region="VTK" ;;
esac


tseries -S $region
