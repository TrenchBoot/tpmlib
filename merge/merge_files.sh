#! /bin/sh
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

mdir=""

if [ "$1" = "lz" ]; then
    mdir=lz
elif [ "$1" = "linux" ]; then
    mdir=linux
else
    echo "Usage: merge_files <lz|linux>"
    exit
fi

rm -r ${mdir}/tpm.h
rm -r ${mdir}/early_tpm.h
rm -r ${mdir}/early_tpm.c
cp ${mdir}/tpm_template.h ${mdir}/tpm.h
cp ${mdir}/early_tpm_template.h ${mdir}/early_tpm.h
cp ${mdir}/early_tpm_template.c ${mdir}/early_tpm.c

echo "/*** tpm.h ***/" >> ${mdir}/tpm.h
./process_file.awk -v header=1 ../include/tpm.h >> ${mdir}/tpm.h
echo "/*** tpmbuff.h ***/" >> ${mdir}/tpm.h
./process_file.awk -v header=1 ../include/tpmbuff.h >> ${mdir}/tpm.h
echo "" >> ${mdir}/tpm.h
echo "#endif" >> ${mdir}/tpm.h
echo "Finished tpm.h"

echo "/*** tpm_common.h ***/" >> ${mdir}/early_tpm.h
./process_file.awk -v header=1 ../tpm_common.h >> ${mdir}/early_tpm.h
echo "/*** tis.h ***/" >> ${mdir}/early_tpm.h
./process_file.awk -v header=1 ../tis.h >> ${mdir}/early_tpm.h
echo "/*** crb.h ***/" >> ${mdir}/early_tpm.h
./process_file.awk -v header=1 ../crb.h >> ${mdir}/early_tpm.h
echo "/*** tpm1.h ***/" >> ${mdir}/early_tpm.h
./process_file.awk -v header=1 ../tpm1.h >> ${mdir}/early_tpm.h
echo "/*** tpm2.h ***/" >> ${mdir}/early_tpm.h
./process_file.awk -v header=1 ../tpm2.h >> ${mdir}/early_tpm.h
echo "/*** tpm2_constants.h ***/" >> ${mdir}/early_tpm.h
./process_file.awk -v header=1 ../tpm2_constants.h >> ${mdir}/early_tpm.h
echo "/*** tpm2_auth.h ***/" >> ${mdir}/early_tpm.h
./process_file.awk -v header=1 ../tpm2_auth.h >> ${mdir}/early_tpm.h
echo "" >> ${mdir}/early_tpm.h
echo "#endif" >> ${mdir}/early_tpm.h
echo "Finished early_tpm.h"

echo "/*** tpm_buff.c ***/" >> ${mdir}/early_tpm.c
./process_file.awk ../tpm_buff.c >> ${mdir}/early_tpm.c
echo "" >> ${mdir}/early_tpm.c
echo "/*** tpmio.c ***/" >> ${mdir}/early_tpm.c
./process_file.awk ${mdir}/tpmio.c >> ${mdir}/early_tpm.c
echo "" >> ${mdir}/early_tpm.c
echo "/*** tis.c ***/" >> ${mdir}/early_tpm.c
./process_file.awk ../tis.c >> ${mdir}/early_tpm.c
echo "" >> ${mdir}/early_tpm.c
echo "/*** crb.c ***/" >> ${mdir}/early_tpm.c
./process_file.awk ../crb.c >> ${mdir}/early_tpm.c
echo "" >> ${mdir}/early_tpm.c
echo "/*** tpm1_cmds.c ***/" >> ${mdir}/early_tpm.c
./process_file.awk ../tpm1_cmds.c >> ${mdir}/early_tpm.c
echo "" >> ${mdir}/early_tpm.c
echo "/*** tpm2_auth.c ***/" >> ${mdir}/early_tpm.c
./process_file.awk ../tpm2_auth.c >> ${mdir}/early_tpm.c
echo "" >> ${mdir}/early_tpm.c
echo "/*** tpm2_cmds.c ***/" >> ${mdir}/early_tpm.c
./process_file.awk ../tpm2_cmds.c >> ${mdir}/early_tpm.c
echo "" >> ${mdir}/early_tpm.c
echo "/*** tpm.c ***/" >> ${mdir}/early_tpm.c
./process_file.awk ../tpm.c >> ${mdir}/early_tpm.c
echo "Finished early_tpm.c"
