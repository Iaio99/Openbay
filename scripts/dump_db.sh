#!/bin/sh

mysqldump -p Openbay > Openbay.sql
git add Openbay.sql
git commit -m "Dump db"
git push
