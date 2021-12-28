if [ "$#" -ne 1 ];  then
    echo -e "Usage: $0 <number of vertices>" >&2
    exit
fi

file_folder="../data/"

num_vertices="$1"
node_file="points${num_vertices}.1.node"
ele_file="points${num_vertices}.1.ele"
neigh_file="points${num_vertices}.1.neigh"
output_off="points${num_vertices}.1.off"
output_hedge="points${num_vertices}.1.hedge"
output_pdf="points${num_vertices}_polylla.1.pdf"

echo -n "Generating mesh with ${num_vertices} vertices..."
cd ../data
./datagenerator.sh "${num_vertices}"
cd ../build
echo "done"

echo -n "Generating mesh..."
make && ./Polylla ${file_folder}${node_file} ${file_folder}${ele_file} ${file_folder}${neigh_file} ${file_folder}${output_off} ${file_folder}${output_hedge}
echo "done"


#echo -n "plotting polyllamesh\n"
#cd ../data
#python3 plot_triangulation.py ${node_file} ${output_hedge} ${output_pdf}
#rm ${output_hedge}
#cd ../build
#echo -n "Done.\n"

