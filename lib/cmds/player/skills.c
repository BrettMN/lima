/* Do not remove the headers from this file! see /USAGE for more info. */

/* Skills command by Tsath */

//: PLAYERCOMMAND
//$$ see: hp, stats, score, spells
// USAGE
//    ``skills``
//    ``skills magic``
//    ``skills combat``
//    ``skills misc``
//    ``skills <enemy>`` (for wizards)
//
// Prints out a list of your skills and skill ranks.
//
// - Skill ranks are shown at the end of the skill names (the higher the better from 0-20)
// - Training points, if used, are shown before the bars.
//
// The more you use your skills, the better you get. So things get better all
// the time! Talk to a trainer to learn more about skills.
// .. TAGS: RST

#include <classes.h>
#include <config/skills.h>

inherit CMD;
inherit CLASS_SKILL;
inherit M_WIDGETS;
inherit M_FRAME;

// Sometimes training down the tree has not managed to propagate to level 2.
// This function manually adds in the level 2 skills if needed.
mapping introduce_level_twos(mapping skills)
{
   foreach (string name, class skill skill in skills)
   {
      string *parts = explode(name, "/");
      if (sizeof(parts) > 2 && !skills[implode(parts[0..1], "/")])
         skills[implode(parts[0..1], "/")] = new (class skill);
   }
   return skills;
}

private
void main(string arg)
{
   mapping skills;
   int width = this_user()->query_screen_width();
   int skill_bar;
   string barchar = uses_unicode() ? "▅" : "=";
   string nobarchar = uses_unicode() ? "▅" : ".";
   string bend = uses_unicode() ? "└" : " ";
   string contbend = uses_unicode() ? "├" : " ";
   string content = "";
   string *names;
   object target;
#ifdef SKILL_CONFIG_USES_TRAINING_PTS
   width -= 4;
#endif
   width -= 2;
   skill_bar = width - 34;

   if (strlen(arg) > 0 && wizardp(this_user()))
   {
      target = present(arg, environment(this_body()));
      if (!target)
         target = find_body(arg);
      if (target)
      {
         out("Skills for " + target->short() + ":\n");
         arg = 0;
      }
   }

   if (!target)
      target = this_body();

   skills = target->query_skills();
   skills = introduce_level_twos(skills);

   if (sizeof(skills) == 0)
   {
      if (target == this_body())
         out("You have no skills yet, do something.\n");
      else
         out(target->short() + " has not skills yet.\n");
      return;
   }
   names = sort_array(keys(skills), 1);

   if (!i_simplify())
   {
      int i = 0;
      while (i < sizeof(names))
      {
         string name = names[i];
         class skill skill = target->query_skill(name);
         string *parts = explode(name, "/");
         int bonus = target->query_skill_bonus(name);
         int level = sizeof(parts);
         int next_level = (i + 1) < sizeof(names) ? sizeof(explode(names[i + 1], "/")) : 0;
         string name2 = repeat_string("   ", sizeof(parts) - 1) + parts[ < 1];
         string pretty_name = target->is_body() ? SKILL_D->skill_rank_pretty(target, name)
                                                : SKILL_D->monster_skill_rank_pretty(target, name);
         int percentage = target->is_body() ? SKILL_D->percent_for_next_rank(target, name)
                                            : SKILL_D->monster_percent_for_next_rank(target, name);
         int green = skill_bar * percentage / 100;
         int red = skill_bar - green;

         frame_init_user();
         if (strlen(arg) && strsrch(name, arg) != 0)
         {
            i++;
            continue;
         }

         if (level == 1)
         {
            if (content != "")
            {
               set_frame_content(content);
               out(frame_render());
            }
            content = "";
            set_frame_title(pretty_name);
         }
         // Screen width > 50
         else if ((percentage || target->is_body()) && width > 50)
            content += sprintf("%-25s " + (bonus > 0 ? "<010>+<res>" : (bonus < 0 ? "<009>-<res>" : " ")) +
                                   " %3.3s [<040>%s<238>%s<res>]\n",
                               repeat_string(" " + (level == next_level ? contbend : bend), level - 2) + pretty_name,
#ifdef SKILL_CONFIG_USES_TRAINING_PTS
                               target->is_body() ? "" + (skill.training_points ? skill.training_points : "") : "",
#else
                               "",
#endif
                               repeat_string(barchar, green), repeat_string(nobarchar, red));
         // Screen width < 50
         else if ((percentage || target->is_body()) && width <= 50)
            content += sprintf(
                "%-25s " + (bonus > 0 ? "<010>+<res>" : (bonus < 0 ? "<009>-<res>" : " ")) + "%4s(%-3s)\n",
                repeat_string(" " + (level == next_level ? contbend : bend), level - 2) + pretty_name, percentage + "%",
#ifdef SKILL_CONFIG_USES_TRAINING_PTS
                target->is_body() ? accent(skill.training_points) : ""
#else
                ""
#endif
            );
         i++;
      }
      if (content)
      {
         set_frame_content(content);
         out(frame_render());
      }
   }
   else
   {
      outf("%-30s %-12s  %-8s\n", "Skill name", "Progress",
#ifdef SKILL_CONFIG_USES_TRAINING_PTS
           "Training points"
#else
           ""
#endif
      );
      foreach (string name in names)
      {
         class skill skill = skills[name];
         int percentage = SKILL_D->percent_for_next_rank(this_body(), name);
         string *parts = explode(name, "/");
         string name2 = repeat_string("   ", sizeof(parts) - 1) + parts[ < 1];
         if (strlen(arg) && strsrch(name, arg) != 0)
            continue;

         if (sizeof(parts) == 1)
         {
            outf("%s:\n", SKILL_D->skill_rank_simple(this_body(), name));
         }
         else
            outf("%-30s %-12s  %-8s\n", SKILL_D->skill_rank_simple(this_body(), name), percentage + "%",
#ifdef SKILL_CONFIG_USES_TRAINING_PTS
                 sizeof(parts) > 1 ? skill.training_points + "" : "-"
#else
                 ""
#endif
            );
      }
   }
}
