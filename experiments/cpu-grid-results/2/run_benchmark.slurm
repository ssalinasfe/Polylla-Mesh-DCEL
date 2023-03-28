#!/bin/bash
#SBATCH --job-name=polylla           # Nombre del trabajo
#SBATCH --output=polylla_%j.out      # Nombre del archivo de salida
#SBATCH --error=polylla_%j.err       # Nombre del archivo de error
#SBATCH --ntasks=1                   # Numero de tareas
#SBATCH --gres=gpu:A100:1            # Asignar un GPU A100

date

FOLDER=$1
NSAMPLES=$(($2-1))

START=1000000
END=100000000

# Definir el tamaño del problema

# Calculamos la diferencia entre los valores extremos y la cantidad de valores que queremos obtener
diff=$(($END-$START))
interval=$((diff/NSAMPLES))

echo "Start: $START, End: $END, Intervalo: $interval, Diferencia: $diff, NSAMPLES: $NSAMPLES"

# Inicializamos un array para almacenar los 5 valores equidistantes con raíz cuadrada entera
values=()

# Iteramos sobre los 5 valores equidistantes
for ((i=$START; i<=$END; i+=interval)); do
    # Calculamos la raíz cuadrada y su parte entera
    sqrt=$(bc <<< "scale=0;sqrt($i)")
    sqrt_int=$(bc <<< "$sqrt/1")

    # Inicializamos las variables para guardar el valor más cercano a la raíz cuadrada entera
    closest_lower=0
    closest_upper=0

    # Iteramos sobre los dos valores enteros más cercanos a la raíz cuadrada
    for ((j=sqrt_int-1; j<=sqrt_int+1; j++)); do
    # Calculamos el cuadrado del valor entero y su distancia al valor original
    j_squared=$((j*j))
    distance=$((j_squared-i))

    # Si este valor es más cercano que los anteriores, lo guardamos
    if [[ $distance -ge 0 ]] && ( [[ $distance -lt $((closest_upper-i)) ]] || [[ $closest_upper -eq 0 ]] ); then
        closest_upper=$j_squared
    elif [[ $distance -lt 0 ]] && ( [[ $((-distance)) -lt $((-closest_lower+i)) ]] || [[ $closest_lower -eq 0 ]] ); then
        closest_lower=$j_squared
    fi
    done

    # Si hay un valor más cercano en la dirección positiva, lo usamos
    if [[ $closest_upper -ne 0 ]] && [[ $((closest_upper-i)) -lt $((i-closest_lower)) ]]; then
    values+=($closest_upper)
    # Si hay un valor más cercano en la dirección negativa, lo usamos
    elif [[ $closest_lower -ne 0 ]]; then
    values+=($closest_lower)
    # Si no hay un valor más cercano en ninguna dirección, usamos el valor original
    else
    values+=($i)
    fi
done

echo "Valores: ${values[@]}"

# Llamamos al programa con los NSAMPLEs valores como argumentos
for value in "${values[@]}"; do
    date

    echo "Ejecutando Polylla con tamaño $value"

    # Medir el tiempo de ejecución del programa Polylla
    TIMEFORMAT="%E"
    ELAPSED_TIME=$(time (srun --container-name=cuda-12.1.0-brew --gres=gpu:A100:1 $FOLDER/Polylla $value >> polylla_${SLURM_JOB_ID}.out) 2>&1 >/dev/null)

    # Imprimir el tiempo de ejecución en los archivos de salida y error
    echo "Tiempo de ejecución: $ELAPSED_TIME segundos" >> polylla_${SLURM_JOB_ID}.out
    echo "Tiempo de ejecución: $ELAPSED_TIME segundos" >> polylla_${SLURM_JOB_ID}.err
    echo "" >> polylla_${SLURM_JOB_ID}.out
    echo "" >> polylla_${SLURM_JOB_ID}.err
done

date