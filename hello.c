

/*
 * Copyright (c) 2017, GlobalLogic Ukraine LLC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by the GlobalLogic.
 * 4. Neither the name of the GlobalLogic nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY GLOBALLOGIC UKRAINE LLC ``AS IS`` AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL GLOBALLOGIC UKRAINE LLC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/ktime.h>
#include <linux/list.h>
#include <linux/slab.h>

struct linked_list {
	struct list_head node;
	ktime_t kernelTime;
};

MODULE_AUTHOR("Serhii Popovych <serhii.popovych@globallogic.com>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

static LIST_HEAD(linked_head);
static uint iter = 1;
module_param(iter, uint, S_IRUGO);
MODULE_PARM_DESC(iter, "uint: number of lines to print");

static int __init hello_init(void)
{
	uint i;
	ktime_t kernelTime;
	struct linked_list *ptr, *tmp;

	BUG_ON(iter>10);


	WARN_ON (iter == 0 || iter >= 5);

	for (i = 0; i < iter; i++) {
		kernelTime = ktime_get();
		printk(KERN_EMERG "Hello, world!\n");
		ptr = kmalloc(sizeof(struct linked_list), GFP_KERNEL);
		
		if (i == 5) {
			ptr = NULL;
		}

		if (ZERO_OR_NULL_PTR(ptr)) {
			goto exitErr;
		}
		*ptr = (struct linked_list) {
			(struct list_head) {NULL, NULL}, kernelTime,
		};
		list_add_tail(&ptr->node, &linked_head);
	}

	return 0;
	exitErr:
	list_for_each_entry_safe(ptr, tmp, &linked_head, node) {
		list_del(&ptr->node);
		kfree(ptr);
	}
	printk(KERN_ERR "Error\n");
	return -1;
}

static void __exit hello_exit(void)
{
	struct linked_list *data, *tmp;

	list_for_each_entry_safe(data, tmp, &linked_head, node) {
		printk(KERN_INFO "%lld\n", data->kernelTime);
		list_del(&data->node);
		kfree(data);
	}
}

module_init(hello_init);
module_exit(hello_exit);
