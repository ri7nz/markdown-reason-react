let component = ReasonReact.statelessComponent("Readme");
let make = children => {
  ...component,
  render: _self =>
    <div>
      <a href="http://lol.to" title="lol"> (ReasonReact.string({|link|})) </a>
      <blockquote>
        (
          ReasonReact.string(
            {|> Lorem ipsum dolor sit amet, instructior vituperatoribus pri cu, quaeque
albucius quaerendum te vel. Zril tamquam maiestatis nam no, in utamur quaestio
scriptorem vim. Tota graeco vel ne, ne vel primis conclusionemque. Mea dicam
tritani reformidans te, at sed erant molestie incorrupte. Laudem iisque ea vis,
solet veniam dissentiet mea et.|},
          )
        )
      </blockquote>
      <a href="http://lol.to" title="lol"> (ReasonReact.string({|link|})) </a>
    </div>,
};
