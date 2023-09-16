/*
 * table.js:
 * Input_field class
 */

class Input_field {
    static input_fields = {}
    dom = null;
    input_field = null;
    label = null;

    // usage: constructor({name: "name", div_id: "id", id: "id", disabled:bool, hidden:bool, label: "label", placeholder:"value", size:int, value:"value"});
    constructor(args) {
        this.name = args.name;
        Input_field.input_fields[args.name] = this;
        this.dom = document.getElementById(args.div_id);
        this.input_field = document.createElement('INPUT');
        this.input_field.type = "text";
        if (args.id)
            this.input_field.id = args.id;
        if (args.disabled)
            this.input_field.disabled = args.disabled;
        if (args.hidden)
            this.hide();
        if (args.placeholder)
            this.input_field.placeholder = args.placeholder;
        if (args.size)
            this.input_field.size = args.size;
        if (args.value)
            this.input_field.value = args.value;
        if (args.label) {
            this.label = document.createElement('LABEL');
            this.label.innerHTML = args.label;
            this.label.appendChild(this.input_field);
            this.dom.appendChild(this.label);
        } else
            this.dom.appendChild(this.input_field);
    }

    // usage: clear();
    clear() {
        this.input_field.value = "";
    }

    get_value() {
        return this.input_field.value;
    }

    hide() {
        this.dom.style.display = 'none';
    }

    set_value(value) {
        this.input_field.value = value;
    }

    show() {
        this.dom.style.display = 'block';
    }
}

