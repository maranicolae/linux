#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
/* TODO: add missing headers */
#include <linux/sched.h>

MODULE_DESCRIPTION("List current processes");
MODULE_AUTHOR("Kernel Hacker");
MODULE_LICENSE("GPL");

struct task_struct *p;

static int my_proc_init(void)
{
	p = current;
	/* TODO: print current process pid and its name */
	pr_info("%s\n", p->comm);
	/* TODO: print the pid and name of all processes */
	pr_info("%d\n", p->pid);
	return 0;
}

static void my_proc_exit(void)
{
	/* TODO: print current process pid and name */
	pr_info("%s\n", p->comm);
	pr_info("%d\n", p->pid);
}

module_init(my_proc_init);
module_exit(my_proc_exit);
