### Rust 优点

**安全：**Rust 是静态的，拥有丰富类型系统和所有权模型，保证了内存安全性和线程安全性。 C 语言很容易出现整数溢出，如果被黑客利用，很容易出现安全问题。而 Rust 中的每个值都只能被一个所有者拥有，所以 C 语言遇到的这类问题，对 Rust 来说都不是问题。

**并发：**并发和并行是 IT 圈内不会过时的话题，Rust 可让程序在编译时并发执行，并且将安全与并发完美统一。

**高效：**没有 Runtime，也没有 GC，所以 Rust 非常快且节省内存，它可以为性能关键型服务提供动力，在嵌入式设备上运行，并且可以轻松地与其他语言集成。

### Rust 缺点

Rust 不能用 trait 来标记一个局部变量和参数的类型，同时 Rust 也不支持子类型。

```rust
fn compare(a: Ord,b: Ord) -> bool{
    a < b
}
```

Rust 可以用 trait object 来表达多态性，但是编译器需要在编译期知道函数的参数、局部变量的大小，所以这里必须套上一个引用或者 Box，引用需要标上生命周期或者让编译器能够推导出生命周期，而 Box 就需要注意闭包捕获的局部变量生命周期可能会不够长。而且编译器对 trait object 有额外的限制：

> Object Safety Is Required for Trait Objects
> You can only make object-safe traits into trait objects. Some complex rules govern all the properties that make a trait object safe, but in practice, only two rules are relevant. A trait is object safe if **all the methods** defined in the trait have the following properties:
>
> - The return type isn’t Self.
> - There are no generic type parameters.