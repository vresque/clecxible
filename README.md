# CLECXIBLE

C-Language, with extensions.

A working C(11) compiler with some extensions, as well as the ability to add (simple) extensions via `.c-ext` files

Example of a `.c-ext` file, which adds namespaces
```c
(set-name "Namespaces")
(set-description "Introduces namespaces to C")
(set-version "0.1")
(runs-after parsing)
(assert-in-domain module) ;; Asserts that it is either in another namespace or in the global scope, but not in functions, etc
(set-pattern
	namespace ?name ?block 
)
(set-param-type ?name ident)
;; Another possible option here
;; (set-param-optional ?block)
(set-param-type ?block block)

(activate (?name ?block) (
	(for_item_in_block it block (
		(push (concat_idents ?name (get-name it))) ;; No local variables, just push, pop, dup and clear
		(set-exported-name it (pop))
		(set-domain it module)
	))
))

```

All that this does is define an extension, called "Namespaces", which matches `namespace IDENT BLOCK` and changes the **exported** name of all items within `BLOCK` to `namespace_name_item_name`.

Example:
```c
namespace abc {
	void hello();
}
```

In this case, `hello` would be exported as `abc_hello`.

## Activating an Extension

There are two ways to activate an extension.

### Via C-Files
```c
#extension <namespaces.c-ext> // Searches the global c-ext dir
```

### Via the CLI (planned)
```bash
clecxc --extension "g:namespaces"
```
