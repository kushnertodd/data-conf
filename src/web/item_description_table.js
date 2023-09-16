/*
 * requests.js:
 * Request class
 */

class Item_description_table {
    static table_name = "item_description";
    static div_id = "item_description_table";
    static id = "item_description_table_id";

    static label;
    static table;

    static buttons;
    static save_button;
    static cancel_button;

    static button_cancel(e) {
        alert("cancel!");
        Table.unselect_row();
        Item_description_table.clear();
    }

    static button_save(e) {
        alert("save!");
        Table.unselect_row();
        Item_description_table.clear();
    }

    static clear() {
        Item_description_table.table.clear();
        Item_description_table.label_unset();
        Item_description_table.buttons.hide();
    }

    static create(item) {
        let headers = ["Item", "Value"];
        let table = Item_description_table.table;
        table.clear();
        table.add_row({id: "r0"})
        let cellWidths = ['50%', '50%'];
        for (let i = 0; i < headers.length; i++)
            table.add_th({
                row_id: "r0",
                id: `h${i}`,
                text: headers[i],
                class_name: " header",
                width: cellWidths[i]
            });
        let name_row_id = "r1";
        let name_tr = table.add_row({data: item.name, id: name_row_id});
        let name_item_td = table.add_td({
            row_id: name_row_id,
            id: "item_name_item",
            text: "name"
        });
        let name_input_td = table.add_td({
            row_id: name_row_id,
            id: "item_name_input",
            text: ""
        });
        let cost_row_id = "r2";
        let name_input = document.createElement("input");
        name_input.value = item.name;
        name_input_td.id = "item_name_input";
        name_input_td.appendChild(name_input);
        let cost_tr = table.add_row({data: item.name, id: cost_row_id})
        let cost_item_td = table.add_td({
            row_id: cost_row_id,
            id: "item_cost_item",
            text: "cost"
        })
        let cost_input_td = table.add_td({
            row_id: cost_row_id,
            id: "item_cost_input",
            text: ""
        });
        let cost_input = document.createElement("input");
        cost_input.value = item.cost;
        cost_input_td.id = "item_cost_input";
        cost_input_td.defaultValue = item.cost;
        cost_input_td.appendChild(cost_input);
        Item_description_table.show(item);
    }

    static init() {
        Item_description_table.table = new Table({
            name: Item_description_table.table_name,
            div_id: Item_description_table.div_id,
            id: Item_description_table.id
        });
        Item_description_table.label = $("#displayed_item");
        Item_description_table.buttons = new Button_set({
            name: "buttons",
            div_id: "item_description_table_buttons",
            hidden: true
        });
        Item_description_table.save_button = new Button({
            name: "Save",
            id: "item_description_table_save",
            event_listener: Item_description_table.button_save
        });
        Item_description_table.cancel_button = new Button({
            name: "Cancel",
            id: "item_description_table_cancel",
            event_listener: Item_description_table.button_cancel
        });
        Item_description_table.buttons.add_button(Item_description_table.save_button);
        Item_description_table.buttons.add_button(Item_description_table.cancel_button);
    }

    static label_set(item_name) {
        Item_description_table.label.html(`Item: ${item_name}`);
    }

    static label_unset() {
        Item_description_table.label.html("");
    }

    static show(item) {
        //Item_description_table.label_set(item.name);
        //Item_description_table.buttons.show();
    }

}
