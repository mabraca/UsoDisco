	/*******************************************************/
	/USO DISCO		CREADO POR:			/
	/			Carlos Noria 			/
	/			Maria Bracamonte		/
	/							/
	/		Lab. Sistema de operacion 		/
	/		Prof.Angela Di Serio. 			/
	/		Prof. Carlos Gomez.			/
	/							/
	/*******************************************************/


INTRODUCCION

	El programa UsoDisco tiene como objetivo calcular de forma concurrente el espacio en disco utilizado por los
archivos regulares a partir de un directorio usando hilos. 


INSTRUCCIONES


1. Descomprima el archivo GrupoK.tar.gz
2. En la consola del computador a utilizar entre en la carpeta /Grupok 
3. Ejecute el comando 'make' en la consola. 
4. Una vez hecho el paso 3, comience la ejecucion del programa con uno de los siguientes comandos: 

./UsoDisco       			
	
	El directorio a tomar sera, por defecto, el directorio actual. 
	Utilizara un solo nivel de concurrencia para trabajar
	Se imprimira en pantalla el resultado

./UsoDisco -h       			
	
	Opcion de ayuda. Imprime por pantalla la sintaxis de como se utiliza el programa.
	
./UsoDisco  -n i			
	
	El directorio a tomar sera, por defecto, el directorio actual. 
	Se imprimira en pantalla el resultado.
	Utilizara i-niveles de concurrencia para trabajar.  i debera ser un numero entero.
	EJEMPLO: 
		./USoDisco n -2
	
./UsoDisco  -n i -d <dir>

	Se imprimira en pantalla el resultado. 
	Utilizara i-niveles de concurrencia para trabajar.  i debera ser un numero entero. 
	El directorio a revisar sera el <dir> que usted desee. <dir> debera ser una direccion. 
	EJEMPLO: 
		./USoDisco n -2 -d /home/usuario/Documentos/Carpetaejemplo

./UsoDisco -n i -d <dir> -o <salida>
	Se imprimira el resultaado el el archivo <salida> que usted desee.  
	Utilizara i-niveles de concurrencia para trabajar.  i debera ser un numero entero. 
	El directorio a revisar sera el <dir> que usted desee. <dir> debera ser una direccion.	
	EJEMPLO: 
		./USoDisco n -2 -d /home/usuario/Documentos/Carpetaejemplo -o Prueba.txt 

	RECORDAR: El orden en el que se coloca el tipo de parametro (-n, -d, -o) puede ser colocado en cualquier orden con la condicion de que el argumento a seguir sea del mismo tipo de parametro referenciado. Es decir, las siguientes instrucciones no son validas: 
	•./usoDisco -h 			Porque el comando se llama UsoDisco
	•./UsoDisco-n -o <salida> 	Falta el nivel de concurrencia i.
	•./UsoDisco -d 4 -o <salida>    Es válido sólo si existe un directorio con nombre 4
	•./usoDisco -h -o salida 	-h No puede ser usado con las otras opciones.


	El archivo de salida lo podra observar en el directorio actual.

FUNCIONAMIENTO 

El comando UsoDisco crea i hilos trabajadores (especificado por la opción “-n i” ). 

El hilo principal (maestro)lleva a cabo las siguientes acciones:

	• Explora el directorio dado a través de la línea de comando y para cada archivo tipo directorio
	encontrado agrega su nombre a la lista de directorios a ser revisados por los hilos trabajadores.

	• Cuando un hilo trabajador está libre, el hilo maestro toma un directorio de la lista de directorios y se lo
	asigna al hilo trabajador libre, elimina el directorio de la lista y marca al hilo trabajador como ocupado.

	• El hilo maestro recibe del hilo trabajador el total de bloques ocupados por los archivos regulares
	encontrados en el directorio explorado y una lista con los nombres de los directorios que encontró en la
	revisión. Estos nombres serán agregados a la lista que el hilo maestro mantiene con los nombres de
	directorio que aún no han sido explorados.

	• Imprime la respuesta enviada por el hilo trabajador

Un hilo trabajador libre lleva a cabo las siguientes acciones:

	• Recibe del hilo maestro el nombre del directorio a explorar

	• Calcula el espacio usado en disco por los archivos regulares dentro de dicho directorio

	• Al terminar, pasa el espacio ocupado y la lista de directorios encontrados al maestro y cambia su estado
	a libre

	• Espera por un nuevo directorio a explorar, y repite los pasos anteriores con el nuevo directorio

La salida indica: directorio examinado (camino completo) y la cantidad de bloques ocupado por los
archivos regulares en dicho directorio.






