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

![20220306181121](https://raw.githubusercontent.com/neicun1024/Interview/main/images_for_markdown/20220306181121.png)

#### Docker

[什么是Docker？生动形象的解释](https://blog.51cto.com/liangchaoxi/4052471?b=totalstatistic)

- Docker容器内的应用程序直接运行在宿主机的内核上，Docker引擎将一些各自独立的应用程序和它们各自的依赖打包，相互独立直接运行于未经虚拟化的宿主机硬件上，同时各个容器也没有自己的内核，比传统虚拟机更轻便


#### K8S
[Kubernetes官方介绍](https://kubernetes.io/zh/docs/concepts/overview/what-is-kubernetes/)

- Kubernetes用于管理这些应用程序所在的小运行环境（容器）
- 一个K8S系统，通常称为一个K8S集群，包括一个Master节点（主节点）和一群Node节点（计算节点）
- Master节点主要负责管理和控制，Node节点时工作负载节点，里面是具体的容器
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