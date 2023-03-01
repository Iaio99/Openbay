#!/bin/sh

mysqldump -p -R Openbay > Openbay.sql
git add Openbay.sql
git commit -m "Dump db"
git push
