/*
 * table.js:
 * Button_set class
 */

class Button_set {
    static button_sets = {};
    dom = null;
    name = null;
    button_set = [];

    // usage: constructor({name: "name", div_id: "div_id", id: "id",  hidden: bool});
    constructor(args) {
        Button_set.button_sets[args.name] = this;
        if (args.name)
            this.name = args.name;
        if (args.div_id)
            this.dom = document.getElementById(args.div_id);
        if (args.id)
            this.name = args.id;
        if (args.hidden)
            this.hide();
    }

    // usage: add_button(button);
    add_button(button) {
        this.button_set.push(button);
        this.dom.appendChild(button.button);
    }

    hide() {
        this.dom.style.display = 'none';
    }

    show() {
        this.dom.style.display = 'block';
    }
}

