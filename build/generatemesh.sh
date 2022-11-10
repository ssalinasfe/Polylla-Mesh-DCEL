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
output="points${num_vertices}.1"

echo -n "Generating mesh with ${num_vertices} vertices..."
cd ../data
./datagenerator.sh "${num_vertices}"
cd ../build
echo "done"

echo -n "Generating mesh..."
make && ./Polylla ${file_folder}${node_file} ${file_folder}${ele_file} ${file_folder}${neigh_file} ${file_folder}${output}
echo "done"