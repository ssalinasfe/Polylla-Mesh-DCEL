node_file="$1.node"
ele_file="$1.ele"
neigh_file="$1.neigh"
output_off="$1.off"
output_hedge="$1.hedge"
#output_pdf="$6.pdf"


echo -n "Generating mesh..."
make && ./Polylla ${node_file} ${ele_file} ${neigh_file} ${output_off}&& geomview ${output_off}
echo "done"
