function AccordionWidget (el, selectedIndex) {
  if (!el) {
    return;
  }

  this.el = el;

  this.accordionItems = this.el.getElementsByClassName('civic-accordion__list-item');
  this.accordionTriggers = this.el.getElementsByClassName('civic-accordion__header__button');
  this.accordionPanels = this.el.getElementsByClassName('civic-accordion__content');

  if (this.accordionTriggers.length === 0 || this.accordionTriggers.length !== this.accordionPanels.length) {
    return;
  }

  this._init(selectedIndex);
}

AccordionWidget.prototype._init = function (selectedIndex) {
  this.accordionTriggersLength = this.accordionTriggers.length;
  this.expandedAccordions = new Array(this.accordionTriggersLength);
  this.multiSelectable = this.el.hasAttribute('aria-multiselectable');
  this.clickListener = this._clickEvent.bind(this);
  this.keydownListener = this._keydownEvent.bind(this);
  this.focusListener = this._focusEvent.bind(this);
  this.keys = {
    prev: 38,
    next: 40,
    space: 32
  };

  var initialSelectedIndex;

  for (var i = 0; i < this.accordionTriggersLength; i++) {
    this.accordionTriggers[i].index = i;
    this.accordionTriggers[i].addEventListener('click', this.clickListener, false);
    this.accordionTriggers[i].addEventListener('keydown', this.keydownListener, false);
    this.accordionTriggers[i].addEventListener('focus', this.focusListener, false);

    if (this.accordionTriggers[i].classList.contains('is-selected')) {
      this.expandedAccordions[i] = true;
    }
  }

  if (!isNaN(selectedIndex)) {
    initialSelectedIndex = selectedIndex < this.accordionTriggersLength ? selectedIndex : this.accordionTriggersLength - 1;
    this.expandedAccordions = new Array(this.accordionTriggersLength);
    this.expandedAccordions[initialSelectedIndex] = true;
  }
  else {
    initialSelectedIndex = this.expandedAccordions.lastIndexOf(true);

    if (!this.multiSelectable) {
      this.expandedAccordions = new Array(this.accordionTriggersLength);
      this.expandedAccordions[initialSelectedIndex] = true;
    }
  }

  this.setSelected(initialSelectedIndex);
  this.setExpanded();
  this.el.classList.add('is-initialized');
};

AccordionWidget.prototype._clickEvent = function (e) {
  e.preventDefault();

  this.setSelected(e.target.index, true);
  this.setExpanded(e.target.index, true);
};

AccordionWidget.prototype._keydownEvent = function (e) {
  var targetIndex;

  switch (e.keyCode) {
    case this.keys.space:
    case this.keys.prev:
    case this.keys.next:
      e.preventDefault();
      break;
    default:
      return;
  }

  if (e.keyCode === this.keys.space) {
    this.setExpanded(e.target.index, true);

    return;
  }
  else if (e.keyCode === this.keys.prev && e.target.index > 0) {
    targetIndex = e.target.index - 1;
  }
  else if (e.keyCode === this.keys.next && e.target.index < this.accordionTriggersLength - 1) {
    targetIndex = e.target.index + 1;
  }
  else {
    return;
  }

  this.setSelected(targetIndex, true);
};

AccordionWidget.prototype._focusEvent = function () {
  if (this.accordionTriggersLength === 1) {
    this.setSelected(0);
  }

  return;
};

AccordionWidget.prototype.setSelected = function (index, userInvoked) {
  if (index === -1) {
    this.accordionTriggers[0].setAttribute('tabindex', 0);

    return;
  }

  for (var i = 0; i < this.accordionTriggersLength; i++) {
    var currentButton = this.accordionTriggers[i];
    if (i === index) {
      currentButton.classList.add('is-selected');
      this.accordionItems[i].classList.add('civic-accordion__list-item--expanded');
      currentButton.classList.add('civic-accordion__header__button--expanded');
      currentButton.parentElement

      currentButton.setAttribute('aria-selected', true);
      currentButton.setAttribute('tabindex', 0);

      if (userInvoked) {
        currentButton.focus();
      }
    }
    else {
      currentButton.classList.remove('civic-accordion__header__button--expanded');
      this.accordionItems[i].classList.remove('civic-accordion__list-item--expanded');
      currentButton.setAttribute('aria-selected', false);
      currentButton.setAttribute('tabindex', -1);
    }
  }
};

AccordionWidget.prototype.setExpanded = function (index, userInvoked) {
  var i;

  if (userInvoked) {
    if (this.multiSelectable) {
      this.expandedAccordions[index] = !this.expandedAccordions[index];
    }
    else {
      for (i = 0; i < this.accordionTriggersLength; i++) {
        if (i === index) {
          this.expandedAccordions[i] = !this.expandedAccordions[i];
        }
        else {
          this.expandedAccordions[i] = false;
        }
      }
    }
  }

  for (i = 0; i < this.accordionTriggersLength; i++) {
    if (this.expandedAccordions[i]) {
      this.accordionTriggers[i].setAttribute('aria-expanded', true);
      this.accordionTriggers[i].classList.add('civic-accordion__header__button--expanded');
      this.accordionItems[i].classList.add('civic-accordion__list-item--expanded');
      this.accordionPanels[i].classList.add('civic-accordion__content--expanded');
      this.accordionPanels[i].style.height = 'auto';

      this.accordionPanels[i].setAttribute('aria-hidden', false);
      this.accordionPanels[i].classList.remove('is-hidden');
      this.accordionPanels[i].setAttribute('tabindex', 0);
    }
    else {
      this.accordionTriggers[i].setAttribute('aria-expanded', false);
      this.accordionTriggers[i].classList.remove('is-expanded');
      this.accordionTriggers[i].classList.remove('civic-accordion__header__button--expanded');
      this.accordionItems[i].classList.remove('civic-accordion__list-item--expanded');
      this.accordionPanels[i].classList.remove('civic-accordion__content--expanded');
      this.accordionPanels[i].style.height = 0;

      this.accordionPanels[i].setAttribute('aria-hidden', true);
      this.accordionPanels[i].classList.add('is-hidden');
      this.accordionPanels[i].setAttribute('tabindex', -1);
    }
  }
};

AccordionWidget.prototype.destroy = function () {
  this.el.classList.remove('is-initialized');

  for (var i = 0; i < this.accordionTriggersLength; i++) {
    this.accordionTriggers[i].removeAttribute('aria-expanded');
    this.accordionTriggers[i].removeAttribute('aria-selected');
    this.accordionTriggers[i].classList.remove('is-expanded');

    this.accordionPanels[i].removeAttribute('aria-hidden');
    this.accordionPanels[i].classList.remove('is-hidden');
    this.accordionPanels[i].removeAttribute('tabindex');

    this.accordionTriggers[i].removeEventListener('click', this.clickListener, false);
    this.accordionTriggers[i].removeEventListener('keydown', this.keydownListener, false);
    this.accordionTriggers[i].removeEventListener('focus', this.focusListener, false);

    delete this.accordionTriggers[i].index;
  }
};

document.addEventListener("DOMContentLoaded", function() {
  const accordions = document.querySelectorAll('.civic-accordion ul');

  Array.from(accordions).forEach((accordion, index) => {
    new AccordionWidget(accordion, index);
  });
});
