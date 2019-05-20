/**
 * brief API implementation for a Queue using an array.
 *
 * @author Rubén Charles
 * @date   21/04/2019
 */

#include "Queue.h"


/**
 * brief	Initializes a queue
 *
 * @param queue	Pointer to structure of type queue_t
 */
void
queue_init (queue_t * queue)
{
	/** No byte is enqueued yet, so this attributes are initialized with zero. */
	queue->first = 0;
	queue->last = 0;
	queue->size = 0;
}

/**
 * @param queue Pointer to structure of type queue_t
 * @return      Number of bytes enqueued
 */
uint8_t
queue_size (queue_t * queue)
{
	return (queue->size);
}

/**
 *
 * @param queue
 * @return If the size of the queue is equal to zero: TRUE
 * 	       Else, return False, queue is not empty.
 */
bool_t
queue_isEmpty (queue_t * queue)
{
	if (NO_BYTES_ENQUEUED == queue->size)
	{
		return (TRUE);
	}
	else		 /** Else added for code readability */
	{
		return (FALSE);		 /** There are bytes enqueued */
	}

}

/**
 * brief  		  Enqueue the the given byte.
 *
 * @param queue   Pointer to structure of type queue_t
 * @param byte_in Data to be enqueued
 */
void
enqueue (queue_t * queue, uint8_t byte_in)
{
	/** This flag is used for knowing which byte was enqueued first*/
	static bool_t first_byte_already_enq_f = FALSE;

	if (FALSE == first_byte_already_enq_f || NO_BYTES_ENQUEUED == queue->size)
	{
		/** Address of the first byte enqueued must be  0 */
		queue->first = 0;
		first_byte_already_enq_f = TRUE;
	}
	/** Now, enqueue the byte received and set it as last enqueued*/
	queue->queue_array[queue->size] = byte_in;
	queue->last = queue->size; /** CHECK */
	/** If the number of elements enqueued exceeds the queue capacity, a rollover occurs
	 * and the queue array is written again from the first position.
	 */
	queue->size = (queue->size +1) % QUEUE_CAPACITY;
}

/**
 * brief  Consult the last byte enqueued.
 *
 * @param queue Pointer to structure of type queue_t
 * @return		Last value enqueued
 */
uint8_t
queue_peek (queue_t * queue)
{
	return (queue->queue_array[queue->last]);
}

/**
 * brief  Dequeue the first byte enqueued
 *
 * @param queue Pointer to structure of type queue_t
 * @return      First byte enqueued
 */
uint8_t
dequeue (queue_t * queue)
{
	/** The new first will be the byte stored just after the first byte */
	queue->size --;
	queue->first = (queue->first + 1)% QUEUE_CAPACITY;
	return (queue->queue_array[queue->first - 1]);
}
