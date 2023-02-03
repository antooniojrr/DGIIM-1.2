#!/bin/bash
# Título: Práctica4
# Fecha: 28/03/2022
# Autor: Antonio Javier Rodríguez Romero
# Descripción: Crea un guion que reciba dos argumentos de tipo numérico y realice las siguientes operaciones.

#    Si el primer argumento es mayor que el segundo, entonces que se multipliquen los valores. En 	este caso, se ha de crear un archivo cuyo nombre contenga el resultado de la multiplicación. 	Por  ejemplo: 12_multi.txt 

#    Si el segundo es mayor que el primero, entonces que se resten.

#    Si son el mismo número, entonces que muestre un mensaje indicándolo simplemente.

# Opciones: Ninguna

numero='^\-?[0-9]+$'
if [ $# != 2 ] ; then printf "Se tienen que introducir solo dos parametros\n" ; exit -1; fi

if ! [[ $1 =~ $numero && $2 =~ $numero ]] ; then printf "Se tienen que introducir dos argumentos numéricos\n"; exit -1; fi

if (( $1 > $2 )) ; then touch $[$1*$2]_multi.txt ; printf "Creado el archivo $[ $1 * $2 ]_multi.txt\n" ; fi

if (( $1 < $2 )) ; then printf "El resultado de restar $1 a $2 es = $[$2-$1]\n" ; fi

if (( $1 == $2 )) ; then printf "Se ha introducido 2 veces el numero $1\n"
fi
