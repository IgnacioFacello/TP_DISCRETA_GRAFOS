Ejemplo

                10(+2)X
        3(-1)                           14(+2)Z
    1(0)                12(0)                           17(-2)
                                                15(+1)
                                                    16(0)
-- Reordenamos

                20(-2)
            5(-1)
        0(0)
   

if ( left_size <  right_size ) // Izq mas grande
    crear un nuevo nodo
    asignarle el tamano

else if ( right_size - left_size > 200 ) // Izq mas grande

Consideramos el peso de la rama

# Rebalanceo

Cuando se modifica el arbol, la diferencia de altura entre dos sub arboles hijos cambia. Mientras que esta diferencia sea < 2 solo debemos adaptar la informacion guardada en el nodo padre (Cambiamos el balance). Si la diferencia es >= 2 debemos realizar una operacion de rotacion.
Las rotaciones son importantes ya que las llaves solo son movidas verticalmente, lo que permite que se mantenga la secuencia de llaves horizontales (in-order).

Dado X con un balance de +2 o -2. Se ha modificado su arbol izquierdo o derecho. Dado Z el hijo mas largo. Ambos hijos se encuentran internamente balanceados.

Si ha ocurrido una insercion, esta ha ocurrido en alguno de los hijos de Z tal que el largo de Z ha aumentado. 
Si ha ocurrido una extracion, esta ha ocurrido en el hermano de Z, de modo que su largo es menor al de Z. (En este caso el balance de Z va a ser 0)

There are four possible variants of the violation:

    Right Right	⟹ Z is a right	child of its parent X and BF(Z) ≥ 0
    Left Left	⟹ Z is a left	child of its parent X and BF(Z) ≤ 0
    Right Left	⟹ Z is a right	child of its parent X and BF(Z) < 0
    Left Right	⟹ Z is a left	child of its parent X and BF(Z) > 0
And the rebalancing is performed differently:
    Right Right	⟹ X is rebalanced with a	simple	rotation rotate_Left	(see figure 2)
    Left Left	⟹ X is rebalanced with a	simple	rotation rotate_Right	(mirror-image of figure 2)
    Right Left	⟹ X is rebalanced with a	double	rotation rotate_RightLeft	(see figure 3)
    Left Right	⟹ X is rebalanced with a	double	rotation rotate_LeftRight	(mirror-image of figure 3)

Tenemos una funcion por caso
