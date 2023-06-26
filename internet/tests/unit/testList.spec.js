import '@testing-library/jest-dom'; //引入 jest-dom 擴充 jest 的斷言方式
import { render, fireEvent } from '@testing-library/vue'; //引入渲染目標元件、模擬使用者行為（事件）的工具
import listToDoList from '../../src/list.vue'; //引入目標元件檔案
import { mount } from '@vue/test-utils'


describe('list.vue', () => {
    //詳細測試目標說明（內容自訂，請盡量清楚明瞭）
    it('check Create button can create a task who contain correct task name', async() => {
        const wrapper = mount(listToDoList);
        const button = wrapper.get('[test-data="create"]').find('[type="button"]');
        wrapper.get('[test-data="taskName"]').setValue("new");
        button.trigger('click');
        expect(wrapper.vm.list.length).toBe(1);
        expect(wrapper.vm.list[0]["message"]).toBe("new");
    });
    it('check Delete All Task button can delete all tasks', async() => {
        const wrapper = mount(listToDoList);
        const button = wrapper.get('[test-data="allTask"]').find('[type="button"]');
        button.trigger('click');
        expect(wrapper.vm.list.length).toBe(0);
    });
    it('check Delete Done Task button can delete all done tasks', async() => {
        const showTest = "new";
        const wrapper = mount(listToDoList);
        const createButton = wrapper.get('[test-data="create"]').find('[type="button"]');
        const deleteButton = wrapper.get('[test-data="doneTask"]').find('[type="button"]');
        createButton.trigger('click');
        wrapper.get('[test-data="taskName"]').setValue(showTest);
        createButton.trigger('click');
        wrapper.vm.list[0]["isActive"] = true;
        expect(wrapper.vm.list.length).toBe(2);
        deleteButton.trigger('click');
        expect(wrapper.vm.list.length).toBe(1);
        expect(wrapper.vm.list[0]["message"]).toBe(showTest);
    });
    it('check change task name can work', async() => {
        const showTest = "new";
        const wrapper = mount(listToDoList);
        const createButton = wrapper.get('[test-data="create"]').find('[type="button"]');
        createButton.trigger('click');
        createButton.trigger('click');
        wrapper.vm.list[1]["time"] = 1;
        var updateMessagePack = [];
        updateMessagePack.push({
            "message": showTest,
            "time": wrapper.vm.list[1]["time"]
        });
        wrapper.vm.fixTaskName(updateMessagePack);
        expect(wrapper.vm.list.length).toBe(2);
        expect(wrapper.vm.list[1]["message"]).toBe(showTest);
    });
    it('check toggle task status can work', async() => {
        const showTest = "new";
        const wrapper = mount(listToDoList);
        const createButton = wrapper.get('[test-data="create"]').find('[type="button"]');
        createButton.trigger('click');
        wrapper.get('[test-data="taskName"]').setValue(showTest);
        createButton.trigger('click');
        wrapper.vm.toggleTaskState(1);
        expect(wrapper.vm.list.length).toBe(2);
        expect(wrapper.vm.list[1]["isActive"]).toBe(true);
    });
    it('check Delete Certain Task can work', async() => {
        const showTest = "new";
        const wrapper = mount(listToDoList);
        const createButton = wrapper.get('[test-data="create"]').find('[type="button"]');
        createButton.trigger('click');
        createButton.trigger('click');
        expect(wrapper.vm.list.length).toBe(2);
        wrapper.vm.list[0]["time"] = 1;
        wrapper.vm.list[1]["message"] = showTest;
        wrapper.vm.deleteCertainTask(wrapper.vm.list[0]["time"]);
        expect(wrapper.vm.list.length).toBe(1);
        expect(wrapper.vm.list[0]["message"]).toBe(showTest);
    });
    it('count Done Tasks and Undone Tasks can work', async() => {
        const wrapper = mount(listToDoList);
        const createButton = wrapper.get('[test-data="create"]').find('[type="button"]');
        createButton.trigger('click');
        createButton.trigger('click');
        expect(wrapper.vm.list.length).toBe(2);
        expect(wrapper.vm.countDoneTasks).toBe(0);
        expect(wrapper.vm.countUndoneTasks).toBe(2);
        wrapper.vm.list[0]["isActive"] = true;
        expect(wrapper.vm.countDoneTasks).toBe(1);
        expect(wrapper.vm.countUndoneTasks).toBe(1);
        wrapper.vm.list[1]["isActive"] = true;
        expect(wrapper.vm.countDoneTasks).toBe(2);
        expect(wrapper.vm.countUndoneTasks).toBe(0);
    });
    it('check decideWhatToShow correct ', async() => {
        const wrapper = mount(listToDoList);
        const createButton = wrapper.get('[test-data="create"]').find('[type="button"]');
        createButton.trigger('click');
        createButton.trigger('click');
        wrapper.vm.show = "all";
        expect(wrapper.vm.decideWhatToShow.length).toBe(2);
        wrapper.vm.show = "done";
        expect(wrapper.vm.decideWhatToShow.length).toBe(0);
        wrapper.vm.show = "undone";
        expect(wrapper.vm.decideWhatToShow.length).toBe(2);
        wrapper.vm.list[0]["isActive"] = true;
        wrapper.vm.show = "all";
        expect(wrapper.vm.decideWhatToShow.length).toBe(2);
        wrapper.vm.show = "done";
        expect(wrapper.vm.decideWhatToShow.length).toBe(1);
        wrapper.vm.show = "undone";
        expect(wrapper.vm.decideWhatToShow.length).toBe(1);
        wrapper.vm.list[1]["isActive"] = true;
        wrapper.vm.show = "all";
        expect(wrapper.vm.decideWhatToShow.length).toBe(2);
        wrapper.vm.show = "done";
        expect(wrapper.vm.decideWhatToShow.length).toBe(2);
        wrapper.vm.show = "undone";
        expect(wrapper.vm.decideWhatToShow.length).toBe(0);

    });




});