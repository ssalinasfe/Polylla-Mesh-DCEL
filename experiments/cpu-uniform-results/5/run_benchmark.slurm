#!/bin/bash
#SBATCH --job-name=polylla           # Nombre del trabajo
#SBATCH --output=polylla_%j.out      # Nombre del archivo de salida
#SBATCH --error=polylla_%j.err       # Nombre del archivo de error
#SBATCH --ntasks=1                   # Numero de tareas
#SBATCH --gres=gpu:A100:1            # Asignar un GPU A100

date

FOLDER=$1
INPUT=$2
NSAMPLES=$(($3))

START=1000000
END=44312500

# Definir el tamaño del problema

# Calculamos la diferencia entre los valores extremos y la cantidad de valores que queremos obtener
diff=$(($END-$START))
interval=$((diff/NSAMPLES))

echo "Start: $START, End: $END, Intervalo: $interval, Diferencia: $diff, NSAMPLES: $NSAMPLES"

# Llamamos al programa con los NSAMPLEs valores como argumentos
for ((i=START; i<=$END; i+=interval)); do
    date

    echo "Ejecutando Polylla con tamaño $i"

    # Medir el tiempo de ejecución del programa Polylla
    TIMEFORMAT="%E"
    ELAPSED_TIME=$(time (srun --container-name=cuda-12.1.0-brew --gres=gpu:A100:1 ${FOLDER}/Polylla ${INPUT}/${i}_points.1.off uniform_${i} >> polylla_${SLURM_JOB_ID}.out) 2>&1 >/dev/null)

    # Imprimir el tiempo de ejecución en los archivos de salida y error
    echo "Tiempo de ejecución: $ELAPSED_TIME segundos" >> polylla_${SLURM_JOB_ID}.out
    echo "Tiempo de ejecución: $ELAPSED_TIME segundos" >> polylla_${SLURM_JOB_ID}.err
    echo "" >> polylla_${SLURM_JOB_ID}.out
    echo "" >> polylla_${SLURM_JOB_ID}.err
done

date
 