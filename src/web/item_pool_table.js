/*
 * requests.js:
 * Request class
 */

class Item_pool_table {
    static table_name = "item_pool";
    static div_id = "item_pool_table";
    static id = "item_pool_table_id";

    static label;
    static table;
    static selected_row;
    static item_name;
    static item_id;

    static buttons;
    static add_item_button;

    static select_add_item_request_failure(req) {
        alert(`select user request failed: '${req}'`);
    }

    static select_add_item_request_success(result) {
        let result_obj = Request.parse_response(result);
        let request_name = result_obj.dconf_request.request;
        //Item_table.label_set();
        //Order_table.add_items_button.enable();
        //Item_table.create(result_obj.dconf_request_response.item_dto_list);
        let select_all_request = new Request({
            request: "order_select_all_items",
            arguments: Order_table.order_id
        });
        select_all_request.send(Item_table.select_order_items_request_success, Item_table.select_order_items_request_failure);
        let select_other_request = new Request({
            request: "order_select_other_items",
            arguments: Order_table.order_id
        });
        select_other_request.send(Item_pool_table.select_order_other_items_request_success, Item_pool_table.select_order_other_items_request_failure);
    }

    static button_add_item(e) {
        e.preventDefault();
        let item_pool_row_selected = Item_pool_table.selected_row;
        let item_id = item_pool_row_selected.data.item_id;
        let order_id = Order_table.order_id;
        let request = new Request({
            request: "order_add_item",
            arguments: [order_id, item_id]
        });
        request.send(Item_pool_table.select_add_item_request_success, Item_pool_table.select_add_item_request_failure);
    }

    static clear() {
        Item_pool_table.table.clear();
        Item_pool_table.label_unset();
        Item_pool_table.buttons.hide();
    }

    static create(item_list) {
        let headers = ["Item", "Cost"];
        let table = Item_pool_table.table;
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
                }
            );
        if (item_list) {
            for (let i = 0; i < item_list.length; i++) {
                let row_id = `r${i}`;
                let tr = table.add_row({data: item_list[i], id: row_id})
                tr.addEventListener('click', Item_pool_table.row_onlick_handler);
                //tr.addEventListener('contextmenu', Item_pool_table.row_contextmenu_onlick_handler);
                table.add_td({row_id: row_id, id: "item_pool_name", text: item_list[i].name})
                table.add_td({row_id: row_id, id: "item_pool_cost", text: item_list[i].cost})
            }
            Item_pool_table.label_set();
            Item_pool_table.buttons.show();
        }
        Item_pool_table.add_item_button.disable();
    }

    static init() {
        Item_pool_table.table = new Table({
            name: Item_pool_table.table_name,
            div_id: Item_pool_table.div_id,
            id: Item_pool_table.id
        });
        Item_pool_table.label = $("#displayed_item_pool");
        Item_pool_table.buttons = new Button_set({
            name: "buttons",
            div_id: "item_pool_table_buttons",
            hidden: true
        });
        Item_pool_table.add_item_button = new Button({
            name: "Add item",
            id: "item_table_add_item_id",
            event_listener: Item_pool_table.button_add_item,
            disabled: true
        });
        Item_pool_table.buttons.add_button(Item_pool_table.add_item_button);
    }

    static label_set() {
        let order_name = Order_table.order_name;
        Item_pool_table.label.html(`Add to order: ${order_name}`);
    }

    static label_unset() {
        Item_pool_table.label.html("");
    }

    static row_onlick_handler(e) {
        e.preventDefault();
        let item_pool_row_selected = e.currentTarget;
        Item_pool_table.selected_row = item_pool_row_selected;
        Item_pool_table.table.select_row(item_pool_row_selected);
        let item = item_pool_row_selected.data;
        Item_pool_table.item_id = item.item_id;
        Item_pool_table.item_name = item.name;
        Item_pool_table.add_item_button.enable();
    }

    static select_order_other_items_request_failure(req) {
        alert(`select user request failed: '${req}'`);
    }

    static select_order_other_items_request_success(result) {
        let result_obj = Request.parse_response(result);
        //let request_name = result_obj.dconf_request.request;
        //Item_table.label_set();
        Item_pool_table.create(result_obj.dconf_request_response.item_dto_list);
    }
}
