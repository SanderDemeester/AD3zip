for i in $(seq 5); do
    sed -n "/blocksize: $i/{N;p}" r >> blocksize_$i
    sed -i 's/^real *\t *0m//g' blocksize_$i
    sed -i 's/^filesize: \([0-9]\{,2\}\).*$/\1/g' blocksize_$i
    sed -i '/^[0-9]\+$/{:a;N;$!ba;s/\n/ /g};s/s /\n/g' blocksize_$i
done
