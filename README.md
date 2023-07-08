# TP 3: Spread toolkit y broadcast tolerante a fallos
*Cátedra*: Virtualización y Sistemas Operativos Avanzados  
*Carrera*: Ingeniería en Sistemas de Información  
*Universidad*: Universidad Tecnológica Nacional - Facultad Regional Santa Fe  
*Docentes*:
- Pessolani, Pablo Andrés
- Harispe, David Gabriel 

*Alumno*: Pacheco Pilan, Federico Ignacio    
*e-mail*: fedepacheco2112@gmail.com  
*Año*: 2023

## Funcionamiento
La aplicación *vsoa_spread_usr* ofrece cuatro funcionalidades: unirse a un grupo, salir de un grupo, enviar un mensaje a un 
grupo mediante un tipo de servicio (no confiable, confiable, fifo, causal, atómico, "seguro"), y recibir (leer) los mensajes 
entrantes encolados. En esta última se discriminan entre mensajes regulares y de membresía (regulares, transicionales, self-leave)
y se muestra en ambos casos información pertinente de los mismos (mensaje, tamaño, miembros restantes, tipo de servicio, etc.).

## Uso
Previamente deben iniciarlizarse los contenedores y a su vez el demonio spread en cada nodo (no se puede ejecutar como root):

``
lxc start ubuntuC-nodei  (i = 1, 2, 3)
``  
``
./spread
``

Luego desde el host compilar y enviar el programa a los nodos:

``
./enviar_programa.sh
``

Finalmente, en cada nodo:

``
./vsoa_spread_usr i  (i = 1, 2, 3)
``

