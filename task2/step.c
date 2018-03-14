#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <signal.h>
#include <syscall.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <sys/user.h>
#include <unistd.h>
#include <errno.h>

/**
 * 使用PTRACE_SINGLESTEP对汇编代码进行单步调试
 * */
int main(int argc, char** argv) {
	if (argc < 2) {
		perror("set a progarm to exec");
		return -1;
	}
	pid_t child_pid = fork();
	if (child_pid == 0) {
		//子进程执行
		/**
		* TODO 子程序执行啥？
		**/
	} else if (child_pid > 0) {
		//父进程执行
		int cnt = 0;
		int status;
		while (1) {
			//等待子进程信号
			wait(&status);
			if (WIFEXITED(status)) //子进程发送退出信号，退出循环
				break;
			cnt++;
			//调用ptrace从子进程取数据
			struct user_regs_struct regs;
			/**
			* TODO 1.使用 PTRACE_GETREGS 获取所有寄存器值存在 &regs 里
			* TODO 2.使用 PTRACE_PEEKTEXT 获取当前指令内容 存在instr里 注意读取的是一个字，64位
			* 虽然X86-64是CISC，指令长度不一定是64位，总之我们就取64位的内存出来看看嘛
			* X86-64中，RIP寄存器用于指向当前执行的指令位置
			* 可以通过regs.rip来访问 RIP寄存器的值
			**/
			printf("[%u] RIP = 0x%016x, Instruction = 0x%016x\n", cnt,
					regs.rip, instr);
			printf("EAX: 0x%08x ", ptrace(PTRACE_PEEKUSER, child_pid, 8 * RAX, NULL));
			printf("EBX: 0x%08x\n", ptrace(PTRACE_PEEKUSER, child_pid, 8 * RBX, NULL));
			printf("ECX: 0x%08x ", ptrace(PTRACE_PEEKUSER, child_pid, 8 * RCX, NULL));
			printf("EDX: 0x%08x\n", ptrace(PTRACE_PEEKUSER, child_pid, 8 * RDX, NULL));
			printf("Press any key to continue...\n");
			getchar();
			/**
			* TODO 使用 PTRACE_SINGLESTEP 来重新启动子进程，执行下一条指令后暂停。
			**/
		}
	} else {
		perror("fork error");
		return -1;
	}

	return 0;
}
