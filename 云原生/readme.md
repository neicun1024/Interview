## 这里记录了我从头开始学习云原生过程中学习到的知识


### 什么是[CNCF](https://www.cncf.io/)？
- CNCF全称Cloud Native Computing Foundation（云原生计算基金会），口号是**坚持和整合开源技术来编排容器作为微服务架构的一部分**
- CNCF作为厂商中立的基金会，致力于Github上的快速成长的开源技术的推广，如Kubernetes、Prometheus、Envoy等，帮助开发人员更快更好地构建出色的产品

### 云原生的架构是什么？什么是云原生？

#### CNCF给出了云原生应用的三大特征：
- 容器化封装：以容器为基础，提高整体开发水平，形成代码和组件重用，简化云原生应用程序的维护。在容器中运行应用程序和进程，并作为应用程序部署的独立单元，实现高水平资源隔离
- 动态管理：通过集中式的编排调度系统来动态地管理和调度
- 面向微服务：明确服务器间的依赖，互相解耦


### 什么是Docker和K8S

#### 背景：
- 在容器技术之前，业界的网红是**虚拟机**，虚拟机技术的代表是VMWare和OpenStack；
- 虚拟机：在物理硬件虚拟成多套硬件后，需要在每套硬件上都部署一个操作系统，接着在这些操作系统上运行相应的应用程序
- 缺点：占用空间大、启动慢、虚拟机软件可能收费

![20220323102426](https://raw.githubusercontent.com/neicun1024/Interview/main/images_for_markdown/20220323102426.png)

#### Docker

[什么是Docker？生动形象的解释](https://blog.51cto.com/liangchaoxi/4052471?b=totalstatistic)

- Docker容器内的应用程序直接运行在宿主机的内核上，Docker引擎将一些各自独立的应用程序和它们各自的依赖打包，相互独立直接运行于未经虚拟化的宿主机硬件上，同时各个容器也没有自己的内核，比传统虚拟机更轻便

![20220323102504](https://raw.githubusercontent.com/neicun1024/Interview/main/images_for_markdown/20220323102504.png)
 
#### K8S
[Kubernetes官方介绍](https://kubernetes.io/zh/docs/concepts/overview/what-is-kubernetes/)

- Kubernetes用于管理这些应用程序所在的小运行环境（容器）
- 一个K8S系统，通常称为一个K8S集群，包括一个Master节点（主节点）和一群Node节点（计算节点）
- Master节点主要负责管理和控制，Node节点是工作负载节点，里面是具体的容器
- Node节点包括Docker、Kubelet、Kube-proxy、Fluentd、kube-dns、Pod
  - Pod是Kubernetes最基本的操作单元，一个Pod代表着集群中运行的一个进程，它内部封装了一个或多个紧密相关的容器
  - Docker负责创建容器
  - Kubelet主要负责监视指派到它所在Node上的Pod，包括创建、修改、监控、删除等
  - Kube-proxy主要负责为Pod对象提供代理
  - Fluentd主要负责日志收集、存储与查询


#### Kubernetes集群

- Kubernetes协调一个高可用计算机集群，每个计算机作为独立单元互相连接工作
- Kubernetes以更高效的方式跨集群自动分发和调度应用容器
- 一个Kubernetes集群包含两种类型的资源：Master调度整个集群、Nodes负责运行应用

![20220306132945](https://raw.githubusercontent.com/neicun1024/Interview/main/images_for_markdown/20220306132945.png)

- Master负责管理整个集群。Master协调集群中的所有活动，例如调度应用、维护应用的所需状态、应用扩容以及推出新的更新
- Node是一个虚拟机或者物理机，它在Kubernetes集群中充当工作机器的角色。每个Node都有Kubelet，



### 目前通用的虚拟化方案有哪些，其对应的优劣及应用场景分别是什么？

[参考](https://www.jianshu.com/p/312056e70387)

#### KVM
- KVM（Kernel-based Virtual Machine）是基于内核的虚拟机，KVM是集成到Linux内核的VMM，是X86架构且硬件支持虚拟化技术(Intel VT或AMD-V)的Linux的全虚拟化解决方案。它是Linux的一个很小的模块，利用Linux做大量的事，如任务调度、内存管理与硬件设备交互等。
- KVM本身不执行任何模拟，需要用户控件程序通过/dev/kvm接口设置一个客户机的虚拟虚拟地址空间，向它提供模拟的I/O，并将其视频显示映射回宿主机的显示屏。
- KVM继承了Linux系统管理内存的诸多特性，比如，分配给虚拟使用的内存可以被交换至交换空间、能够使用大内存页以实现更好的性能，以及对NUMA的支持能够让虚拟机高效访问更大的内存空间等。
- KVM基于Intel的EPT（ExtendedPage Table）或AMD的RVI（Rapid Virtualization Indexing）技术可以支持更新的内存虚拟功能，这可以降低CPU的占用率，并提供较好的吞吐量。此外，KVM还借助于KSM（Kernel Same-pageMerging）这个内核特性实现了内存页面共享。KSM通过扫描每个虚拟机的内存查找各虚拟机间相同的内存页，并将这些内存页合并为一个被各相关虚拟机共享的单独页面。在某虚拟机试图修改此页面中的数据时，KSM会重新为其提供一个新的页面副本。实践中，运行于同一台物理主机上的具有相同GuestOS的虚拟机之间出现相同内存页面的概率是很的，比如共享库、内核或其它内存对象等都有可能表现为相同的内存页，因此，KSM技术可以降低内存占用进而提高整体性能。

#### Xen
- Xen是一个基于X86架构、发展最快、性能最稳定、占用资源最少的开源虚拟化技术。在Xen使用的方法中，没有指令翻译。这是通过两种方法之一实现的。第一，使用一个能理解和翻译虚拟操作系统发出的未修改指令的CPU（此方法称作完全虚拟化）。另一种，修改操作系统，从而使它发出的指令最优化，便于在虚拟化环境中执行（此方法称作准虚拟化）。
- 在Xen环境中，主要有两个组成部分。一个是虚拟机监控器（VMM）。VMM层在硬件与虚拟机之间，是必须最先载入到硬件的第一层。Hypervisor载入后，就可以部署虚拟机了。在Xen中，虚拟机叫做“domain”。在这些虚拟机中，其中一个扮演着很重要的角色，就是domain0，具有很高的特权。通常，在任何虚拟机之前安装的操作系统才有这种特权。
- Domain0要负责一些专门的工作。由于hypervisor中不包含任何与硬件对话的驱动，也没有与管理员对话的接口，这些驱动就由domain0来提供了。通过domain0，管理员可以利用一些Xen工具来创建其它虚拟机（Xen术语叫domainU）。这些domainU也叫无特权domain。这是因为在基于i386的CPU架构中，它们绝不会享有最高优先级，只有domain0才可以。

#### Hyper-V
- Hyper-V采用微内核的架构，兼顾了安全性和性能的要求。Hyper-V底层的VMM运行在最高的特权级别下，微软将其称为ring -1（而Intel则将其称为root mode），而虚拟机的OS内核和驱动运行在ring 0，应用程序运行在ring 3下，这种架构就不需要采用复杂的BT（二进制特权指令翻译）技术，可以进一步提高安全性。
- 由于Hyper-V底层的VMM代码量很小，不包含任何第三方的驱动，非常精简，所以安全性更高。Hyper-V采用基于VMbus的高速内存总线架构，来自虚机的硬件请求（显卡、鼠标、磁盘、网络），可以直接经过VSC，通过VMbus总线发送到根分区的VSP，VSP调用对应的设备驱动，直接访问硬件，中间不需要Hypervisor的帮助。
- 这种架构效率很高，不再像以前的Virtual Server，每个硬件请求，都需要经过用户模式、内核模式的多次切换转移。更何况Hyper-V现在可以支持Virtual SMP，Windows Server 2008虚机最多可以支持4个虚拟CPU；而Windows Server 2003最多可以支持2个虚拟CPU。每个虚机最多可以使用64GB内存，而且还可以支持X64操作系统。