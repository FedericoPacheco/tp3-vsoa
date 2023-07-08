#!/bin/bash

sudo gcc -o vsoa_spread_usr vsoa_spread_usr.c -I./SPREAD/include -L./SPREAD/lib -lspread

scp vsoa_spread_usr ubuntuadm@10.62.245.196:/home/ubuntuadm
scp vsoa_spread_usr ubuntuadm@10.62.245.189:/home/ubuntuadm
scp vsoa_spread_usr ubuntuadm@10.62.245.123:/home/ubuntuadm
