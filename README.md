# 3DN-TP-FreeRTOS

1.1 Tâche simple
1. En quoi le paramètre TOTAL_HEAP_SIZE a-t-il de l’importance ?
	R. Il définit la quantité de mémoire disponible pour créer des tâches, des files de messages ou des sémaphores dans FreeRTOS.
2. Quel est le rôle de la macro portTICK_PERIOD_MS ?
	R. Elle sert à convertir des millisecondes en "ticks" pour FreeRTOS.
	Par exemple, vTaskDelay(100 / portTICK_PERIOD_MS) attend 100 ms.