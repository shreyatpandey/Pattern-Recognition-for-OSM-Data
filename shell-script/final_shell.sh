declare -a array=("pretoria.png" "polokwane.png" "sandton.png" "bisho.png" "kimberley.png" "port_elizabeth.png" "alexandra.png" "johannesburg.png" "capetown.png" "khayelitsha.png")

arraylength=${#array[@]}
echo $arraylength
k=0

for (( i=0; i<${arraylength}; i++ ));
 
do
 for (( j=i; j<${arraylength}; j++ ));

  do
     for k in $(seq 0.00 22.5 360)
do

echo "Rotation_angle:"$k

#g++  rotate.cpp -o outrotate `pkg-config --cflags --libs opencv` -std=c++11

./outrotate ${array[$j]} ${array[$j]}$i$j$k.png $k

#g++  houghUpdate.cpp -o outhough `pkg-config --cflags --libs opencv` -std=c++11

./outhough ${array[$j]}$i$j$k.png ${array[$j]}$i$j$k.txt

#g++  RMSEupdate.cpp -o outrmse `pkg-config --cflags --libs opencv` -std=c++11

line1=$(head -n 1 ${array[$i]}.txt)
echo $line1
line2=$(head -n 1 ${array[$j]}$i$j$k.txt)
echo $line2
if [ "$line2" -gt "$line1" ]; 
then
./outrmse ${array[$i]}.txt ${array[$j]}$i$j$k.txt
 else
./outrmse ${array[$j]}$i$j$k.txt ${array[$i]}.txt
fi
if [ "$i" -eq "$j" ];
then
       name="0"
       echo $name>>final$i$j.txt
else
    while IFS= read -r line # Read a line
    do
       name="$line"
       echo $name>>final$i$j.txt
    done <"rmse.txt"
fi

done
done
done
echo "Minimum rmse value is:"
for (( i=0; i<${arraylength}; i++ ));
do
 echo "Row is:"$i
 for (( j=$i; j<${arraylength}; j++ ));
 do

getArray() {
    array=() # Create array
    while IFS= read -r line # Read a line
    do
        array+=("$line") # Append line to the array
    done < "$1"
}

getArray final$i$j.txt


echo "${array[*]}" |tr ' ' '\n'| awk 'NR==1{min=$0}NR>1 && $1<min{min=$1;pos=NR}END{print min}'

done

done


