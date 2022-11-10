if [ "$#" -ne 1 ];  then
    echo -e "Usage: $0 <number of vertices>" >&2
    exit
fi

num_vertices="$1"

random_pts="points${num_vertices}.node"
node_file="points${num_vertices}.1.node"
ele_file="points${num_vertices}.1.ele"
neigh_file="points${num_vertices}.1.neigh"


#Generating random points
echo -n "Generating data\n"
python3 10000x10000RandomPoints.py $1 > $random_pts
echo -n "Done.\n"

echo -n "Generating triangulation\n"
triangle -n $random_pts
echo -n "Done.\n"

rm ${random_pts}