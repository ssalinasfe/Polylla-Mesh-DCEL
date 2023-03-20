#!/bin/bash
#SBATCH --job-name=polylla           # Nombre del trabajo
#SBATCH --output=polylla_%j.out      # Nombre del archivo de salida
#SBATCH --error=polylla_%j.err       # Nombre del archivo de error
#SBATCH --ntasks=1                   # Numero de tareas

# Definir el tamaño del problema
SIZE=$1

echo "Ejecutando Polylla con tamaño $SIZE"

# Medir el tiempo de ejecución del programa Polylla
TIMEFORMAT="%E"
ELAPSED_TIME=$(time (srun --container-name=cuda-12.1.0-brew -p cpu ../build/Polylla $SIZE >> polylla_${SLURM_JOB_ID}.out) 2>&1 >/dev/null)

# Imprimir el tiempo de ejecución en los archivos de salida y error
echo "Tiempo de ejecución: $ELAPSED_TIME segundos" >> polylla_${SLURM_JOB_ID}.out
echo "Tiempo de ejecución: $ELAPSED_TIME segundos" >> polylla_${SLURM_JOB_ID}.err


# Finalizar el trabajo
exit 0
