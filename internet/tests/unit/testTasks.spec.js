import '@testing-library/jest-dom'; //引入 jest-dom 擴充 jest 的斷言方式
import { render, fireEvent } from '@testing-library/vue'; //引入渲染目標元件、模擬使用者行為（事件）的工具
import tasksComponents from '../../src/components/tasks.vue'; //引入目標元件檔案
import { mount } from '@vue/test-utils'


describe('list.vue', () => {
    //詳細測試目標說明（內容自訂，請盡量清楚明瞭）
    it('check toggle task status can emit', async() => {
        const testTime = 1;
        const testMessage = 'temp';
        const testIndex = 2;
        const wrapper = mount(tasksComponents, {
            propsData: {
                message: testMessage,
                active: false,
                index: testIndex,
                time: testTime
            }
        })
        const button = wrapper.get('[test-data="toggleTaskState"]').find('[type="button"]');
        button.trigger('click');
        expect(wrapper.emitted('toggleTaskState')[0][0]).toBe(2);
    });
    it('check Delete Certain Task can emit', async() => {
        const testTime = 1;
        const testMessage = 'temp';
        const testIndex = 2;
        const wrapper = mount(tasksComponents, {
            propsData: {
                message: testMessage,
                active: false,
                index: testIndex,
                time: testTime
            }
        })
        const button = wrapper.get('[test-data="deleteTask"]').find('[type="button"]');
        button.trigger('click');
        expect(wrapper.emitted('deleteCertainTask')[0][0]).toBe(1);
    });
    it('check Change Certain Task Name and Input can emit', async() => {
        const testTime = 1;
        const testMessage = 'temp';
        const testIndex = 2;
        const messageNew = 'new';
        const wrapper = mount(tasksComponents, {
            propsData: {
                message: testMessage,
                active: false,
                index: testIndex,
                time: testTime
            }
        })
        const button = wrapper.get('[test-data="changeTaskName"]').find('[type="button"]');
        wrapper.get('[test-data="tasksName"]').setValue(messageNew);
        button.trigger('click');
        console.log(wrapper.emitted('fixTaskName')[0][0][0]);
        expect(wrapper.emitted('fixTaskName')[0][0][0]["time"]).toBe(1);
        expect(wrapper.emitted('fixTaskName')[0][0][0]["message"]).toBe(messageNew);
    });
    it('check Done tasks and Undone Tasks can show', async() => {
        const testTime = 1;
        const testMessage = 'temp';
        const testIndex = 2;
        const messageNew = 'new';
        var wrapper = mount(tasksComponents, {
            propsData: {
                message: testMessage,
                active: false,
                index: testIndex,
                time: testTime
            }
        })
        expect(wrapper.find(".done").exists()).toBe(false)
        wrapper = mount(tasksComponents, {
            propsData: {
                message: testMessage,
                active: true,
                index: testIndex,
                time: testTime
            }
        })
        expect(wrapper.find(".done").exists()).toBe(true)
    });



});