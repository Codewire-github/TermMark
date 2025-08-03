# Building a Simple Todo App with JavaScript

Creating a todo application is one of the best ways to learn how frontend development works. In this tutorial, we'll walk through building a **minimal** todo app using just **HTML**, **CSS**, and **JavaScript**.

## Table of Contents

1.  Introduction
2.  Setup
3.  HTML Structure
4.  Styling with CSS
5.  JavaScript Logic
6.  Final Thoughts

-----

## 1. Introduction

The goal is to create an app that allows users to:

  - Add a new todo
  - Mark it as complete
  - Delete it from the list

Here’s what it’ll look like when done:

> “A clean, simple list of todos with buttons to manage them. Nothing fancy, just functionality.”

-----

## 2. Setup

Let’s begin by setting up a basic HTML file. You can name it `index.html`.

```html
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <title>Todo App</title>
    <link rel="stylesheet" href="style.css" />
  </head>
  <body>
    <div id="app"></div>
    <script src="script.js"></script>
  </body>
</html>
```

-----

## 3. HTML Structure

Inside the `#app` div, we’ll render a simple input and list container.

```html
<div class="todo-container">
  <input type="text" id="todo-input" placeholder="Enter a task" />
  <button id="add-btn">Add Todo</button>
  <ul id="todo-list"></ul>
</div>
```

-----

## 4. Styling with CSS

We'll use very basic styles for this example.

```css
.todo-container {
  width: 300px;
  margin: auto;
  font-family: sans-serif;
}
.completed {
  text-decoration: line-through;
  color: gray;
}
```

-----

## 5. JavaScript Logic

Let’s write the core logic of our app. Save this as `script.js`.

```javascript
document.getElementById("add-btn").addEventListener("click", () => {
  const input = document.getElementById("todo-input");
  const text = input.value.trim();
  if (text === "") return;

  const li = document.createElement("li");
  li.textContent = text;

  li.addEventListener("click", () => {
    li.classList.toggle("completed");
  });

  li.addEventListener("contextmenu", (e) => {
    e.preventDefault();
    li.remove();
  });

  document.getElementById("todo-list").appendChild(li);
  input.value = "";
});
```

### Note:

  - Click to **toggle** completed state.
  - **Right-click** to delete a task.

-----

## 6. Final Thoughts

Building small projects like this helps solidify your understanding of how JavaScript interacts with the DOM. You can further enhance this app by:

  - Storing todos in `localStorage`
  - Adding due dates
  - Using a JS framework like React

> “The best way to learn is to build. Keep experimenting\!”

-----

Here's a useful link: [GitHub](https://github.com).

*Thanks for reading\!* 📝

**Happy Coding\!**

-----

## Key Components Summary

| Component | Purpose | File |
| :--- | :--- | :--- |
| **HTML** | Defines the structure and content of the web page | `index.html` |
| **CSS** | Controls the styling and visual presentation of the app | `style.css` |
| **JavaScript** | Handles the interactive logic and dynamic behavior | `script.js` |
