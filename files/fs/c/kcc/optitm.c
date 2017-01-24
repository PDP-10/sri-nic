/* This code could be optimized to something much better.
** keep it around as a reminder.
*/

extern struct id_str_s { char *s_cp; int s_len; } idistr();
 
item_exists(item)
int item;
{
    struct id_str_s  itmbuf;

    itmbuf = idistr(item);
    return (itmbuf.s_len != 0);
}
