#!/bin/sh

for i in abcd_*
do
  if test -f $i
  then
    base=`echo $i | sed -e 's,abcd_,,'`
    mv abcd_$base abcd_$base
  fi
done

for i in *
do
  if test -f $i
  then
  echo $i
cat <<EOF | ed $i
1,\$s,abcd,abcd,g
1,\$s,aBCD,aBCD,g
1,\$s,ABCD,ABCD,g
w
q
EOF
  fi
done
