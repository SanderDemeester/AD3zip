for i in $(seq 5); do
    sed -n "/blocksize: $i/{N;p}" r >> blocksize$i
    sed -i 's/^real *\t *0m//g' blocksize$i
    sed -i 's/^filesize: \([0-9]\{,2\}\).*$/\1/g' blocksize$i
    sed -i '/^[0-9]\+$/{:a;N;$!ba;s/\n/ /g};s/s /\n/g' blocksize$i
done
cat $1|tr '\n' ' ' |sed 's/s/s\n/g' >> $1_b
rm $1