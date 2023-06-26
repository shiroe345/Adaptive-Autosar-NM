import '@testing-library/jest-dom'; //引入 jest-dom 擴充 jest 的斷言方式
import { render, fireEvent } from '@testing-library/vue'; //引入渲染目標元件、模擬使用者行為（事件）的工具
import buttonDecideElementToShow from '../../src/components/decideElementToShow.vue'; //引入目標元件檔案
import { mount } from '@vue/test-utils'

function decideWhichShowTasksButtonToChoose(condition) {
    var choose;
    if (condition == 'all') choose = 0;
    else if (condition == 'done') choose = 1;
    else if (condition == 'undone') choose = 2;
    else choose = 3;
    const wrapper = mount(buttonDecideElementToShow);
    const button = wrapper.findAll('[type="button"]').at(choose);
    return {
        'button': button,
        'wrapper': wrapper
    };
}

describe('decideElementToShow.vue', () => {
    //詳細測試目標說明（內容自訂，請盡量清楚明瞭）
    it('check Show All Tasks button can pass correct value', async() => {
        const buttonAndWrapper = decideWhichShowTasksButtonToChoose('all');
        buttonAndWrapper['button'].trigger('click');
        expect(buttonAndWrapper['wrapper'].emitted('setShowValue')[0]).toEqual(['all'])
    });

    it('check Show Done Tasks button can pass correct value', async() => {
        const buttonAndWrapper = decideWhichShowTasksButtonToChoose('done');
        buttonAndWrapper['button'].trigger('click');
        expect(buttonAndWrapper['wrapper'].emitted('setShowValue')[0]).toEqual(['done'])
    });

    it('check Show Undone Tasks button can pass correct value', async() => {
        const buttonAndWrapper = decideWhichShowTasksButtonToChoose('undone');
        buttonAndWrapper['button'].trigger('click');
        expect(buttonAndWrapper['wrapper'].emitted('setShowValue')[0]).toEqual(['undone'])

    });


});