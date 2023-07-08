#!/bin/bash

sudo gcc -o vsoa_spread_usr vsoa_spread_usr.c -I/mnt/hgfs/TP3/Material/SPREAD/includeSpread -L/mnt/hgfs/TP3/Material/SPREAD/libSpread -lspread
scp vsoa_spread_usr ubuntuadm@10.62.245.196:/home/ubuntuadm
scp vsoa_spread_usr ubuntuadm@10.62.245.189:/home/ubuntuadm
scp vsoa_spread_usr ubuntuadm@10.62.245.123:/home/ubuntuadm
