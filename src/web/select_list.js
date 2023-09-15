/*
 * table.js:
 * Select_list class
 */

class Select_list {
    static select_lists = {};
    dom = null;
    name = null;
    options = {};
    select_list = null;

    // usage: constructor({name: "name", div_id: "id", id: "id", event_listener: eventListener});
    constructor(args) {
        this.name = args.name;
        Select_list.select_lists[args.name] = this;
        this.dom = document.getElementById(args.div_id);
        this.select_list = document.createElement('SELECT');
        this.dom.appendChild(this.select_list);
        if (args.event_listener) addEventListener('change', args.event_listener);
    }

    // usage: add_option({id: "id", data: obj, text: "text", value: "value", selected: bool, disabled: bool});
    add_option(args) {
        let option = document.createElement('OPTION');
        this.select_list.options.add(option)
        option.select_list = this;
        if (args.id) option.id = args.id;
        if (args.data) option.data = args.data;
        if (args.text) option.text = args.text;
        if (args.value) option.value = args.value;
        if (args.selected) option.selected = args.selected;
        if (args.disabled) option.disabled = args.disabled;
        if (args.id) this.options[args.id] = option;
        return option;
    }

    // usage: clear();
    clear() {
        let select_list = this.select_list;
        for (let i = select_list.length; i > 0; i--) {
            select_list.remove(i - 1);
        }
    }

    get_length() {
        return this.select_list.length;
    }

    get_option_by_index(index) {
        return this.select_list.item[index];
    }

    get_selected_index() {
        return this.select_list.selectedIndex;
    }

    get_selected_option() {
        if (this.select_list.selectedIndex === -1)
            return null;
        else
            return this.get_option_by_index(get_selected_index());
    }

    remove(index) {
        this.select_list.remove(index);
    }

    static select_option(index) {
        this.select_list.selectedIndex = index;
    }

    static unselect_options() {
        this.select_list.selectedIndex = -1
    }
}

