# How to contribute to simplectemplate

First of all, thank you for taking the time to contribute to this project. We're trying to make a stable project and try to fix bugs and add new features continuously. You can help us do more.

If there is a bug or a feature that is not listed in the **issues** page or there is no one assigned to the issue, feel free to fix/add it! Please discuss the change you wish to make first via issue, email, or any other method with the owners of this repository before making a change. This is not **required** but users who discuss will take priority over those who simply submit a pull request.

Please note we have a Pull Request Process, please follow it in all your interactions with the project.

## Fixing issues and Pull Request Process

* Fork this report
* Creating a new feature / bug branch based off the `develop` branch
* Fix the bug on your feature / bug branch.

### **Did you find a bug?**

* **Ensure the bug was not already reported** by searching on GitHub under [Issues](https://github.com/dafky2000/simplectemplate/issues).
* If you're unable to find an open issue addressing the problem, [open a new one](https://github.com/dafky2000/simplectemplate/issues/new). Be sure to include a **title and clear description**, as much relevant information as possible, and a **code sample** or an **executable test case** demonstrating the expected behavior that is not occurring.

### **Did you write a patch that fixes a bug?**

* Ensure your code follows the project coding style, described below.
* Ensure you have created unit tests for the new code.
* Open a new GitHub pull request with the patch.
* Ensure the PR description clearly describes the problem and solution. Include the relevant issue number if applicable.
* Ensure your PR passes the automated Travis CI build process.
* We will let you know if there is any problem or any changes that should be considered.

### Running Tests

simplectemplate utilizes [speck](https://github.com/compiler-dept/speck) for unit tests. It is simple and straight forward, please look at `spec/example.c` for some example implementations. All test file and function names should follow the same naming convention `file_name_function_name.c`.

You will also require valgrind for the unit tests as that is part of the testing process.

```shell
git clone https://github.com/dafky2000/simplectemplate.git
git submodule init
git submodule update
make test
```

### **Coding Style**

* Use tabs, not spaces. If you like 4-space tabs, set your ```tabstop``` appropriately. Some (sane) people, like myself ;), like 2-space indentation and tabs suit everyone.
* Who cares about line width.. Comments, text and other non-readability impacting stuff (not code) can rely on word wrapping.
* All code should follow K&R style, with some caveats:
  * Opening braces should always be on the same line, even on function definitions.
  * You may omit braces for one-line if-statements, else, etc.
  
### Documentation

Every chunk of code that may be hard to understand has some comments above it. If you write some new code or change some part of the existing code in a way that it would not be functional without changing it's usages, it needs to be documented.

# **Do you have questions about the source code?**

simplectemplate is a volunteer effort. We encourage you to pitch in and [join the team](https://github.com/dafky2000/simplectemplate/graphs/contributors)!

Thank You! :heart: :heart: :heart:

CONTRIBUTING file inspired/based on [rails contrubiting](https://github.com/rails/rails/blob/master/CONTRIBUTING.md)
