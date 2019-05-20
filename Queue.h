/**
 * brief API header for a Queue using an array.
 *
 * @author Rubén Charles
 * @date   21/04/2019
 */


#ifndef QUEUE_H_
#define QUEUE_H_


#include "MK64F12.h"
#include "GPIO.h"
// void (*fifo_enqueue_function)(uint8_t data)
/** Set the size of the Queue to use*/
#define QUEUE_CAPACITY 50
/** For knowing if the Queue is empty */
#define NO_BYTES_ENQUEUED 0



typedef struct
{
    uint8_t queue_array[QUEUE_CAPACITY];
    uint16_t first; /** First byte enqueued */
    uint16_t last;  /** Last byte  enqueued */
	uint16_t size; /** How many elements are enqueued */
}queue_t;

/**
 * brief	Initializes a queue
 *
 * @param queue	Pointer to structure of type queue_t
 */
void queue_init(queue_t * queue);

/**
 * @param queue Pointer to structure of type queue_t
 * @return      Number of bytes enqueued
 */
uint8_t queue_size(queue_t * queue);
/**
 *
 * @param queue
 * @return If the size of the queue is equal to zero: TRUE
 * 	       Else, return False, queue is not empty.
 */
bool_t queue_isEmpty(queue_t * queue);
/**
 * brief  		  Enqueue the the given byte.
 *
 * @param queue   Pointer to structure of type queue_t
 * @param byte_in Data to be enqueued
 */
void enqueue(queue_t * queue, uint8_t byte_in);
/**
 * brief  Consult the last byte enqueued.
 *
 * @param queue Pointer to structure of type queue_t
 * @return		Last value enqueued
 */
uint8_t  queue_peek(queue_t * queue);
/**
 * brief  Dequeue the first byte enqueued
 *
 * @param queue Pointer to structure of type queue_t
 * @return      First byte enqueued
 */
uint8_t  dequeue(queue_t  * queue);


#endif /* QUEUE_H_ */
