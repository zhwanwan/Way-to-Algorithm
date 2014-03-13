//二叉树动规
//binary_tree.cpp

//一棵二叉树，共n个节点从1到n，根节点为1
//每个节点上都有一个权值，为方便考虑，所有权值都是正整数
//现在给定一个数量m，少于节点总数
//求出只保留m个节点的树，其余的节点都裁掉，所能保留的最大权值
//当然保留的节点必须连接着根节点，裁掉的都是边缘节点
//为了方便考虑，我们这一整章中所有节点的权值都大于0
//题目的原型是“加分二叉树”，原题中拥有权值的是树的边而非树节点
//但本文为了更方便描绘准确的数学模型，并没有把权值赋给树的边而是赋给树节点

//树型动态规划的最为不同之处是使用递归技术来遍历树空间
//而不像普通动归中使用循环技术来遍历数组那样直观
//
//设f[i][j]指代以i节点为根节点的树上，保留j个节点(包括根节点自己)的最大权值
//状态转移方程：
//f[i][j] = max(f[left[i]][k] + f[right[i]][j- k - 1] + t[k])
//t[k]为第k个节点的权值，0 <= k <= j
//left[i]为节点i的左孩子节点，right[i]为i的右节点
//初始条件：f[i][0]为0，0 <= i <= n，没有节点的子树权值为0
//f[i][1] = s[i]，以i为根节点的最小子树的权值等于自己根节点的权值
//
//本节引用了“不撞南墙不回头——树规总结”，作者“焦作一中信息学oy”

//1)本文第一种方法是我自己的发明，在网上没有类似方法
//该方法将动归常用的f数组也转化成节点的一个成员数组t_f
//请仔细体会这个数据结构，它更加完整的体现了树型动态规划中选择空间的分层本质
//最初我本来准备用这样的数据结构完成这一节的所有代码
//可是在后面的扩展问题中，该数据结构越来越成为函数接口的累赘，导致程序难以调试
//所以本文只用它实现最简单的二叉树动规，后面不再使用这种复杂的数据结构

#include "general_head.h"
#include "tree_dynamic.h"

int binary_tree1(tree_node *r, int resv)
{//二叉树根节点为r，其中每个节点的权值数组t_f初始化为0，权值为t_value
 //二叉树保留resv个节点，返回二叉树最大权值
	//递归终止条件
	//处理叶子节点
	if(r == NULL)
		return(0);
	//若resv为0，则以该r节点为根节点的子树上没有孩子节点
	if(resv == 0)
		return(0);
	//用于动规记录的f[][]数组存储在节点的t_f，所有节点的t_f初始化为0
	//t_f[i]指代以当前节点r为根节点的子树上保留i个节点的最大权值
	//当前r保留resv节点的最大权值已经得到，直接返回
	//这是动态规划的核心体现：
	//对之前已经得到的决策结果不再重复计算，而是存储起来直接查询
	if(r->t_f[resv])
		return(r->t_f[resv]);

	//枚举左子树和右子树保留节点的个数 0 <= i < resv
	//左子树分配剩余的i个节点
	//右子树必然剩余 resv-i-1 个节点(根节点自己 1 个)
	for(int i = 0; i < resv; ++ i){
		int lmax = binary_tree1(r->t_lc, i);
		int rmax = binary_tree1(r->t_rc, resv - i - 1);
		//lmax指代左子树保留i个节点的最大权值
		//rmax指代右子树保留resv-i-1个节点的最大权值
		//当r可以保留resv个节点，它给左子树和右子树分配节点的方法有resv-1种
		//遍历这resv-1种方法，选取最大的之和即为r分配resv个节点的最大权值
		r->t_f[resv] = max(r->t_f[resv], lmax + rmax);
	}
	//对于以r为根节点的树
	//r树最大权值 = 左子树最大权值 + 右子树最大权值 + 根节点权值
	r->t_f[resv] += r->t_value;
	return(r->t_f[resv]);
}

//2)第二种方法中使用了常规的数组f，简单清晰

int binary_tree2(tree_node *r, int **f, int resv)
{//初始时二维数组f中所有成员均为0
	//递归终止条件
	if(r == NULL)
		return(0);
	//若r节点保留resv个节点的决策已经得到，直接查询返回
	if(f[r->t_idx][resv])
		return(f[r->t_idx][resv]);
	if(resv == 0)
		return(0);

	for(int i = 0; i < resv; ++ i){
		int lmax = binary_tree2(r->t_lc, f, i);
		int rmax = binary_tree2(r->t_rc, f, resv - i - 1);
		f[r->t_idx][resv] = max(f[r->t_idx][resv], lmax + rmax);
	}
	f[r->t_idx][resv] += r->t_value;
	return(f[r->t_idx][resv]);
}



