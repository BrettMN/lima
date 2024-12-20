/* Do not remove the headers from this file! see /USAGE for more info. */

inherit VERB_OB;

//: COMMAND
// USAGE
//
//  |  ``unlock <something>``
//  |  ``unlock <something> with <something>``
//
//  Unlock the door, unlock it using a key.
//
// .. TAGS: RST

void do_unlock_obj_with_obj(object ob1, object ob2)
{
   ob1->do_unlock(ob2);
}

void do_unlock_obj(object ob1)
{
   ob1->do_unlock();
}

void create()
{
   add_rules(({"OBJ", "OBJ with OBJ"}));
}
