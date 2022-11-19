PAV - P3: estimación de pitch
=============================

Esta práctica se distribuye a través del repositorio GitHub [Práctica 3](https://github.com/albino-pav/P3).
Siga las instrucciones de la [Práctica 2](https://github.com/albino-pav/P2) para realizar un `fork` de la
misma y distribuir copias locales (*clones*) del mismo a los distintos integrantes del grupo de prácticas.

Recuerde realizar el *pull request* al repositorio original una vez completada la práctica.

Ejercicios básicos
------------------

- Complete el código de los ficheros necesarios para realizar la estimación de pitch usando el programa
  `get_pitch`.

   * Complete el cálculo de la autocorrelación e inserte a continuación el código correspondiente.
      - Insertamos codigo del calculo de la autocorrelacion:
        ```
        for (unsigned int l = 0; l < r.size(); ++l) {
          for(unsigned int n=0; n < x.size()-l;n++)
            r[l] += x[n]* x[n+l];
          r[l] = r[l] / x.size();
        }
        if (r[0] == 0.0F) //to avoid log() and divide zero
          r[0] = 1e-10;
        ```

   * Inserte una gŕafica donde, en un *subplot*, se vea con claridad la señal temporal de un segmento de
     unos 30 ms de un fonema sonoro y su periodo de pitch; y, en otro *subplot*, se vea con claridad la
	 autocorrelación de la señal y la posición del primer máximo secundario.

	 NOTA: es más que probable que tenga que usar Python, Octave/MATLAB u otro programa semejante para
	 hacerlo. Se valorará la utilización de la biblioteca matplotlib de Python.

      - Hemos utilizado un script de matlab, para realizar los plots, dónde hemos escogido un segmento de 30ms manualmente el qual es completamente sonoro, del que le hacemos la autocorrelación de dicho en el mismo script. Mostramos en el subplot superior la senyal y en el inferior su autocorrelación.
      ![IMAGEN](https://github.com/polserra-lab/P3/blob/main/MATLAB.png) 
      Insertamos codigo del script de matlab. Cabe destacara que este analisis, lo hacemos con el archivo de la base de datos test, en concreto rl001.wav.
      ```
        x=importdata("rl001.wav");
        x=x.data;
        x_30ms=x(1719:(1719+599),1); %escogemos 600 muestras sonoras
        t=(0:1/20000:(600/20000-1/20000)); %600 muestras a fs=20000 equivale a unos 30 ms
        subplot(211)
        plot(t,x_30ms);
        subplot(212)
        r=xcorr(x_30ms);
        r_recortada=r(600:1199); %cogemos solo la mitad positiva de la autocorrelacion
        plot(t,r_recortada);
      ```
      - Podemos ver en la imagen insertada en la parte superior, como el periodo de pitch en la señal original es de 0,00735s, mientras que el proporcionada por el segundo maximo de la autocorrelación es de 0,00725s .

   * Determine el mejor candidato para el periodo de pitch localizando el primer máximo secundario de la
     autocorrelación. Inserte a continuación el código correspondiente.
      - - Para determinar el segundo màximo de la autocorrelación, hemos desarrollado el siguente codigo: 
        
        ```
          for(iRMax = iR = r.begin() + npitch_min; iR<r.begin()+npitch_max; iR++){
            if(*iR>*iRMax)
            iRMax=iR;
          }
        ```

   * Implemente la regla de decisión sonoro o sordo e inserte el código correspondiente.
      - Para determinar si la trama es sorda tenemos en cuenta el valor de autocorrelación normalizada (rmaxnorm) además del valor del cociente de r[1]/r[0](La optimización de este último parametro se ha hecho de forma manual). Tal como les mostramos en el código siguiente:
      ```
        if((r1norm>0.75)&&rmaxnorm > umaxnorm ) return false;
        return true;
      ```

   * Puede serle útil seguir las instrucciones contenidas en el documento adjunto `código.pdf`.

- Una vez completados los puntos anteriores, dispondrá de una primera versión del estimador de pitch. El 
  resto del trabajo consiste, básicamente, en obtener las mejores prestaciones posibles con él.

  * Utilice el programa `wavesurfer` para analizar las condiciones apropiadas para determinar si un
    segmento es sonoro o sordo. 
	
	  - Inserte una gráfica con la estimación de pitch incorporada a `wavesurfer` y, junto a ella, los 
	    principales candidatos para determinar la sonoridad de la voz: el nivel de potencia de la señal
		  (r[0]), la autocorrelación normalizada de uno (r1norm = r[1] / r[0]) y el valor de la
		  autocorrelación en su máximo secundario (rmaxnorm = r[lag] / r[0]).
      ![IMAGEN](https://github.com/polserra-lab/P3/blob/main/WAVESURFER_1.png)
      Podemos observar como en los tramos en que wavesurfer estima el picth en la grafica superior, son lo tramos considerados sonoros por el programa, se puede ver que por norma general, los tramos en los que no estima su pitch (sordos) suelen tener potencia más baja.

		Puede considerar, también, la conveniencia de usar la tasa de cruces por cero.

	  Recuerde configurar los paneles de datos para que el desplazamiento de ventana sea el adecuado, que
		en esta práctica es de 15 ms.

      - Use el estimador de pitch implementado en el programa `wavesurfer` en una señal de prueba y compare
	    su resultado con el obtenido por la mejor versión de su propio sistema.  Inserte una gráfica
		  ilustrativa del resultado de ambos estimadores.
      ![IMAGEN](https://github.com/polserra-lab/P3/blob/main/WAVESURFER_2.png)
      En la grafica de arriba, se ve el resultado de nuestro detector de pitch, y en la del miedo, el de wavesurfer. (Como en el apartado de matlab también usamos el archivo de pitch_db/test/rl001.wav).
     
		Aunque puede usar el propio Wavesurfer para obtener la representación, se valorará
	 	el uso de alternativas de mayor calidad (particularmente Python).
  
  * Optimice los parámetros de su sistema de estimación de pitch e inserte una tabla con las tasas de error
    y el *score* TOTAL proporcionados por `pitch_evaluate` en la evaluación de la base de datos 
	`pitch_db/train`..

Ejercicios de ampliación
------------------------

- Usando la librería `docopt_cpp`, modifique el fichero `get_pitch.cpp` para incorporar los parámetros del
  estimador a los argumentos de la línea de comandos.
  
  Esta técnica le resultará especialmente útil para optimizar los parámetros del estimador. Recuerde que
  una parte importante de la evaluación recaerá en el resultado obtenido en la estimación de pitch en la
  base de datos.

  * Inserte un *pantallazo* en el que se vea el mensaje de ayuda del programa y un ejemplo de utilización
    con los argumentos añadidos.

- Implemente las técnicas que considere oportunas para optimizar las prestaciones del sistema de estimación
  de pitch.

  Entre las posibles mejoras, puede escoger una o más de las siguientes:

  * Técnicas de preprocesado: filtrado paso bajo, diezmado, *center clipping*, etc.
  * Técnicas de postprocesado: filtro de mediana, *dynamic time warping*, etc.
  * Métodos alternativos a la autocorrelación: procesado cepstral, *average magnitude difference function*
    (AMDF), etc.
  * Optimización **demostrable** de los parámetros que gobiernan el estimador, en concreto, de los que
    gobiernan la decisión sonoro/sordo.
  * Cualquier otra técnica que se le pueda ocurrir o encuentre en la literatura.

  Encontrará más información acerca de estas técnicas en las [Transparencias del Curso](https://atenea.upc.edu/pluginfile.php/2908770/mod_resource/content/3/2b_PS%20Techniques.pdf)
  y en [Spoken Language Processing](https://discovery.upc.edu/iii/encore/record/C__Rb1233593?lang=cat).
  También encontrará más información en los anexos del enunciado de esta práctica.

  Incluya, a continuación, una explicación de las técnicas incorporadas al estimador. Se valorará la
  inclusión de gráficas, tablas, código o cualquier otra cosa que ayude a comprender el trabajo realizado.
    - Hemos optado por implementar un técnica de preprocesado (center clipping) y otra de postprocesado (filtro de mediana).
    - 1) Center clipping: Hemos implementado la tecnica de center clipping con offset. La cual consiste en a partir de un umbral normalizado respecto a la amplitud màxima de la senyal (para que sea efectivo con senyales con gran amplitud como con pequeña) a partir de un cierto umbral (finalmente en nuestro caso 0.01) recorar la señal, y las otras restarles el umbral en caso de que sea positivo el valor o sumarle en caso de que su valor sea negativo. Se puede ver en la implementación del cogigo que adjuntamos a continuación:
    ```
      vector<float>::iterator iX;
      float max=0;
      for(iX=x.begin(); iX < x.begin()+(int) x.size(); iX++){
        if(abs(*iX) > max)
          max=*iX;
      }
      float th=0.01*max;
      for(iX=x.begin(); iX < x.begin()+(int) x.size(); iX++){
        if(abs(*iX)<th){
          *iX=0;
        }else{
          if(*iX>0){
            *iX=*iX-th;
          }else{
            *iX=*iX+th;
          }
        }
      }
    ```
    - 2) Filtro de mediana: Para tratar de eliminar errores groseros del resultado final, como pueden ser la estimación de un multiplo de la frecuencia de pitch real o bien la de un submultiplo, hemos implementado un filtro de mediana como postprocesado del resultado final. La implementación ha sido de la siguiente manera:
      ```
        int F_size = 1;  
        vector<float> filter;
        for(iX = f0.begin(); iX<f0.end()-(F_size-1); iX += 1){
          // fill filter    
          for(int i = 0; i<F_size; i++)      
            filter.push_back(*(iX+i));
          // Order filter  ​
          int k, l;

          for(k = 0; k < F_size-1; k++){
            for(l = 0; l < F_size-k-1; l++){
              if (filter[l] > filter[l+1]){        
                float aux = filter[l];        
                filter[l] = filter[l+1]; 
                filter[l+1] = aux;      
              }    
            }   
          }
          // Get median    
          f0[iX - f0.begin()] = filter[F_size/2];
          filter.clear();  
        }
      ```
 
  También se valorará la realización de un estudio de los parámetros involucrados. Por ejemplo, si se opta
  por implementar el filtro de mediana, se valorará el análisis de los resultados obtenidos en función de
  la longitud del filtro.
   

Evaluación *ciega* del estimador
-------------------------------

Antes de realizar el *pull request* debe asegurarse de que su repositorio contiene los ficheros necesarios
para compilar los programas correctamente ejecutando `make release`.

Con los ejecutables construidos de esta manera, los profesores de la asignatura procederán a evaluar el
estimador con la parte de test de la base de datos (desconocida para los alumnos). Una parte importante de
la nota de la práctica recaerá en el resultado de esta evaluación.
